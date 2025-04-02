#include "SyslinuxSetup.hpp"
#include <cstdlib>
#include <iostream>

bool SyslinuxSetup::verifyOrInstallSyslinux() {
  // TODO:: Check if syslinux is installed, and if not, install it.
  return false;
}
bool SyslinuxSetup::copySyslinuxBootFiles(
    const std::string bootfilesSourceList,
    const std::string &bootfilesOutputDir) {
  // TODO given the output directory, copy all required syslinux files to that
  // directory. additionally, should have a way of choosing the required files.
  // Maybe string list from a listings file. syslinux.txt
  return false;
}
