#!/bin/bash

# Resolve script directory to find the config file relative to the script location
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
CONFIG_FILE="$SCRIPT_DIR/../config/macvlan.conf"

# Check if the config file exists
if [ ! -f "$CONFIG_FILE" ]; then
  echo "Error: Configuration file $CONFIG_FILE not found."
  exit 1
fi

# Source the configuration file
source "$CONFIG_FILE"

# Verify required keys are present
if [ -z "$interface_name" ] || [ -z "$physical_interface" ] || [ -z "$connection_ip_address" ] || [ -z "$subnet_mask" ]; then
  echo "Error: One or more required keys are missing in $CONFIG_FILE."
  echo "Required keys: interface_name, physical_interface, connection_ip_address, subnet_mask"
  exit 1
fi

# Check if the parent interface exists
if ! ip link show "$physical_interface" >/dev/null 2>&1; then
  echo "Error: Parent interface $physical_interface does not exist."
  exit 1
fi

# Delete the interface if it already exists
sudo ip link delete "$interface_name" >/dev/null 2>&1

# Create the macvlan interface
echo "Creating macvlan interface $interface_name linked to $physical_interface..."
sudo ip link add "$interface_name" link "$physical_interface" type macvlan mode bridge

# Assign the IP address to the new interface
echo "Assigning IP address $connection_ip_address/$subnet_mask to $interface_name..."
sudo ip addr add "$connection_ip_address/$subnet_mask" dev "$interface_name"

# Bring the interface up
echo "Bringing up the interface $interface_name..."
sudo ip link set "$interface_name" up

# Display the new interface status
echo "Interface $interface_name created successfully:"
ip addr show "$interface_name"
