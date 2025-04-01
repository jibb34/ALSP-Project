#!/bin/bash

# Define bridge and network interfaces
BRIDGE_NAME="br0"
PHYSICAL_INTERFACE="enp14s0"
MACVTAP_NAME="macvtap0"

# Define a MAC address for your VM (ensure it's unique within your network)
VM_MAC="52:54:00:12:34:56"

# Amount of memory to allocate to the VM
VM_MEMORY="2048"

# CPU to use (host for maximum compatibility)
VM_CPU="host"

# Create bridge if it doesn't exist
if ! ip link show "$BRIDGE_NAME" >/dev/null 2>&1; then
  echo "Creating network bridge: $BRIDGE_NAME"
  sudo ip link add name $BRIDGE_NAME type bridge
  sudo ip link set $BRIDGE_NAME up
fi

# Add physical interface to bridge if not already a member
if ! brctl show $BRIDGE_NAME | grep -q "$PHYSICAL_INTERFACE"; then
  echo "Adding $PHYSICAL_INTERFACE to bridge $BRIDGE_NAME"
  sudo ip link set $PHYSICAL_INTERFACE down
  sudo brctl addif $BRIDGE_NAME $PHYSICAL_INTERFACE
  sudo ip link set $PHYSICAL_INTERFACE up
fi

# Ensure the bridge is up
sudo ip link set $BRIDGE_NAME up

# Create macvtap interface if it doesn't exist
if ! ip link show "$MACVTAP_NAME" >/dev/null 2>&1; then
  echo "Creating macvtap interface: $MACVTAP_NAME"
  sudo ip link add link $PHYSICAL_INTERFACE name $MACVTAP_NAME type macvtap mode bridge
  sudo ip link set $MACVTAP_NAME up
fi

# Ensure macvtap device is up
sudo ip link set $MACVTAP_NAME up

# Check if user is in the kvm group
if ! groups $(whoami) | grep -q "\bkvm\b"; then
  echo "Adding $(whoami) to the kvm group..."
  sudo usermod -aG kvm $(whoami)
  echo "Please log out and log back in or use 'newgrp kvm' to apply group changes."
fi

echo "Permissions and network interfaces have been set up correctly."
echo "Starting QEMU VM with PXE boot..."

# Start QEMU VM using bridge mode
sudo qemu-system-x86_64 \
  -enable-kvm \
  -m "$VM_MEMORY" \
  -cpu "$VM_CPU" \
  -netdev bridge,id=net0,br=$BRIDGE_NAME \
  -device virtio-net-pci,netdev=net0,mac="$VM_MAC" \
  -boot n -nographic
