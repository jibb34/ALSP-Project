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

# Copy necessary files from Arch Linux system (pxelinux.0 and ldlinux.c32)
echo "Copying PXE boot files from Arch Linux system..."
cp /usr/lib/syslinux/bios/pxelinux.0 "$OUTPUT_DIR/"
cp /usr/lib/syslinux/bios/ldlinux.c32 "$OUTPUT_DIR/"

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
  local url=$1
  local output_path=$2

  if [ -f "$output_path" ]; then
    echo "$output_path already exists, skipping download."
  else
    echo "Downloading ISO from $url to $output_path..."
    wget -O "$output_path" "$url"
  fi
}

# Function to mount ISO, extract files, and unmount
extract_files_from_iso() {
  local iso_path=$1
  local kernel_path=$2
  local initrd_path=$3
  local kernel_output=$4
  local initrd_output=$5
  local iso_name=$(basename "$iso_path")
  local mount_point="/mnt/${iso_name%.iso}"

  echo "Mounting $iso_path to $mount_point..."
  sudo mkdir -p "$mount_point"
  sudo mount -o loop "$iso_path" "$mount_point"

  if [ -f "$mount_point$kernel_path" ]; then
    cp "$mount_point$kernel_path" "$OUTPUT_DIR/$kernel_output"
    echo "Extracted kernel to $OUTPUT_DIR/$kernel_output"
  else
    echo "Kernel file not found at $mount_point$kernel_path"
  fi

  if [ -f "$mount_point$initrd_path" ]; then
    cp "$mount_point$initrd_path" "$OUTPUT_DIR/$initrd_output"
    echo "Extracted initrd to $OUTPUT_DIR/$initrd_output"
  else
    echo "Initrd file not found at $mount_point$initrd_path"
  fi

  sudo umount "$mount_point"
  sudo rmdir "$mount_point"
}

# Generate PXE configuration file
echo "Generating PXE configuration file..."
cat <<EOF >"$OUTPUT_FILE"
DEFAULT ${config[global.default_option]}
PROMPT 0
TIMEOUT ${config[global.timeout]}
MENU TITLE ${config[global.boot_menu_title]}
EOF

# Loop over each defined OS section
for section in "${!config[@]}"; do
  if [[ "$section" =~ \.iso_name$ ]]; then
    os="${section%%.*}"

    iso_name="${config[$os.iso_name]}"
    base_url="${config[$os.base_url]}"
    kernel_path="${config[$os.kernel_path]}"
    initrd_path="${config[$os.initrd_path]}"
    append="${config[$os.append]}"
    label="${config[$os.label]}"

    iso_path="$ISO_DIR/$iso_name"

    # Download the ISO if necessary
    download_iso "$base_url/$iso_name" "$iso_path"

    # Extract kernel and initrd files
    extract_files_from_iso "$iso_path" "$kernel_path" "$initrd_path" "$(echo ${os} | tr '[:upper:]' '[:lower:]')-vmlinuz" "$(echo ${os} | tr '[:upper:]' '[:lower:]')-initrd.img"

    # Add entry to PXE config file
    cat <<EOF >>"$OUTPUT_FILE"
LABEL $os
    MENU LABEL $label
    KERNEL ${os}-vmlinuz
    APPEND initrd=${os}-initrd.img $append
EOF
  fi
done

echo "PXE configuration file generated successfully at $OUTPUT_FILE"

# Automatically rebuild the Docker container to apply changes
echo "Rebuilding TFTP Docker container to apply changes..."
docker-compose build tftp
docker-compose up -d tftp

echo "TFTP Server is now running with updated configuration."
