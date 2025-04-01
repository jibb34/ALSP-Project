#!/bin/bash

# Resolve the script directory to always find the correct paths
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

# Define paths relative to the script directory
CONFIG_FILE="$SCRIPT_DIR/../config/bootfiles.conf"
OUTPUT_DIR="$SCRIPT_DIR/../containers/tftp/bootfiles"
BOOTFILES_DIR="$OUTPUT_DIR/pxelinux.cfg"
OUTPUT_FILE="$BOOTFILES_DIR/default"
ISO_DIR="$OUTPUT_DIR/iso"

# Ensure output directories exist
mkdir -p "$OUTPUT_DIR" "$BOOTFILES_DIR" "$ISO_DIR"

echo "Copying PXE boot files from Arch Linux system..."
cp /usr/lib/syslinux/bios/pxelinux.0 "$OUTPUT_DIR/"
cp /usr/lib/syslinux/bios/ldlinux.c32 "$OUTPUT_DIR/"
cp /usr/lib/syslinux/bios/menu.c32 "$OUTPUT_DIR/"
cp /usr/lib/syslinux/bios/libcom32.c32 "$OUTPUT_DIR/"
cp /usr/lib/syslinux/bios/libutil.c32 "$OUTPUT_DIR/"

# Read the configuration file and parse sections
declare -A config
current_section=""

while IFS= read -r line || [[ -n "$line" ]]; do
  line=$(echo "$line" | sed 's/^\s*//; s/\s*$//') # Trim whitespace
  if [[ -z "$line" || "$line" =~ ^# ]]; then
    continue
  elif [[ "$line" =~ ^\[(.*)\]$ ]]; then
    current_section="${BASH_REMATCH[1]}"
    config[$current_section]=""
  elif [[ "$line" =~ ^(.+?)=(.+)$ ]]; then
    key="${BASH_REMATCH[1]}"
    value="${BASH_REMATCH[2]}"
    config["$current_section.$key"]="$value"
  fi
done <"$CONFIG_FILE"

# Function to download ISO if not already present
download_iso() {
  local iso_name=$1
  local base_url=$2
  local iso_path="$ISO_DIR/$iso_name"

  if [ -f "$iso_path" ]; then
    echo "$iso_path already exists, skipping download."
  else
    if [ -n "$base_url" ]; then
      echo "Downloading ISO from $base_url/$iso_name to $iso_path..."
      wget -O "$iso_path" "$base_url/$iso_name"
      if [ $? -ne 0 ]; then
        echo "Error downloading $iso_name from $base_url. Exiting."
        exit 1
      fi
    else
      echo "No URL specified for $iso_name. Ensure the ISO file is manually placed in the iso directory."
    fi
  fi
}

extract_files_from_iso() {
  local iso_path=$1
  local kernel_path=$2
  local initrd_path=$3
  local kernel_output=$4
  local initrd_output=$5
  local iso_name=$(basename "$iso_path")

  # Use /tmp/iso_mount instead of /mnt
  local mount_root="/tmp/iso_mount"
  local mount_point="$mount_root/${iso_name%.iso}"

  echo "Mounting $iso_path to $mount_point..."
  sudo mkdir -p "$mount_point" || {
    echo "Error creating $mount_point. Exiting."
    exit 1
  }

  sudo mount -o loop "$iso_path" "$mount_point" 2>/dev/null
  if [ $? -ne 0 ]; then
    echo "Failed to mount $iso_path to $mount_point"
    echo "Check if $mount_point is writable or if the ISO is valid."
    return
  fi

  # Check for kernel file
  if [ -f "$mount_point/$kernel_path" ]; then
    sudo cp "$mount_point/$kernel_path" "$OUTPUT_DIR/$kernel_output"
    echo "Extracted kernel to $OUTPUT_DIR/$kernel_output"
  else
    echo "Kernel file not found at $mount_point/$kernel_path"
  fi

  # Check for initrd file
  if [ -f "$mount_point/$initrd_path" ]; then
    sudo cp "$mount_point/$initrd_path" "$OUTPUT_DIR/$initrd_output"
    echo "Extracted initrd to $OUTPUT_DIR/$initrd_output"
  else
    echo "Initrd file not found at $mount_point/$initrd_path"
  fi

  sudo umount "$mount_point"
  sudo rmdir "$mount_point"
}

echo "Generating PXE configuration file..."

{
  echo "DEFAULT menu.c32"
  echo "PROMPT 0"
  echo "TIMEOUT 100"
  echo "MENU TITLE PXE Boot Menu"
  echo
} >"$OUTPUT_FILE"

# 1) Build a list of unique "OS sections" that have an .iso_name
declare -A processed_oses=()

for section in "${!config[@]}"; do
  # If the key ends with ".iso_name", we interpret it as one OS
  if [[ "$section" =~ \.iso_name$ ]]; then
    # For example, if section="rocky.iso_name", then os="rocky"
    os="${section%%.*}"

    # Skip if we already processed this OS
    if [[ -n "${processed_oses[$os]}" ]]; then
      continue
    fi
    processed_oses[$os]=1

    # Pull all relevant config
    iso_name="${config[$os.iso_name]}"
    base_url="${config[$os.base_url]}"
    kernel_path="${config[$os.kernel_path]}"
    initrd_path="${config[$os.initrd_path]}"
    append="${config[$os.append]}"
    label="${config[$os.label]}"

    # Convert OS name to lowercase for filenames
    os_lower="$(echo "${os}" | tr '[:upper:]' '[:lower:]')"

    cat <<EOF >>"$OUTPUT_FILE"
LABEL $os
    MENU LABEL $label
    KERNEL ${os_lower}-vmlinuz
    APPEND initrd=${os_lower}-initrd.img ip=dhcp ${append}
EOF

    echo "Added PXE entry for $os"
  fi
done

echo "PXE configuration file generated successfully at $OUTPUT_FILE"

# Automatically rebuild the Docker container to apply changes
echo "Rebuilding TFTP Docker container to apply changes..."
docker-compose build tftp
docker-compose up -d tftp

echo "TFTP Server is now running with updated configuration."
