#!/usr/bin/env bash
sudo qemu-system-x86_64 \
  -netdev tap,id=net0,ifname=tap0,script=no,downscript=no \
  -device e1000,netdev=net0 \
  -boot n \
  -m 2048 \
  -enable-kvm \
  -serial mon:stdio \
  -nographic
