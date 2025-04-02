#include "PxelinuxConfigGenerator.hpp"
#include "ConfigLoader.hpp"
#include <fstream>
#include <iostream>
#include <vector>

bool PxelinuxConfigGenerator::generateDefaultConfig(
    const ConfigLoader &config) {
  /* TODO: Implement Function:
   * read [global] tags (global.tftpDir) to get base TFTP Directory
   * =========================================
   * create the pxelinux.cfg directory if it doesn't already exist
   * =========================================
   * write the "default" file.
   * =========================================
   * write the basic header:
   * DEFAULT menu.c32
   * PROMPT 0
   * TIMEOUT 100
   * MENU TITLE PXE Boot Menu
   * =========================================
   * For each:
   *  OS Label: create a menu entry from the sections
   *  i.e.:
   *  os_version.label
   *  os_version.kernel_name
   *  os_version.initramfs_name
   *  os_version.append
   *
   * We can figure out what to do later with append... this is important for
   * actually loading from the HTTP server.
   * =========================================
   * insert the values into the template for an entry:
   *
   * LABEL <os_version>
   * MENU <label or os_version (depending on what is better)>
   * KERNEL <kernel_name>
   * INITRD <init_ramfs_name>
   * APPEND <append>
   */
  return false;
}
