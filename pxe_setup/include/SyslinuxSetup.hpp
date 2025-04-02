#pragma once
#include <string>

class SyslinuxSetup {
public:
  bool verifyOrInstallSyslinux(); // Verifies if syslinux is in fact installed
                                  // on the system

  // copies the required syslinux bootfiles specified in bootfiles.conf
  bool copySyslinuxBootFiles(const std::string bootfilesSourceList,
                             const std::string &bootfilesDir);
};
