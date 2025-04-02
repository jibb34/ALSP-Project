#include "SyslinuxSetup.hpp"
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool SyslinuxSetup::verifyOrInstallSyslinux() {
  // check if the syslinux binary is in the PATH, if so, check = 0
  int check = std::system("which syslinux > /dev/null 2>&1");
  if (check == 0) {
    std::cout << "[SyslinuxSetup] syslinux found in PATH" << std::endl;
    return true;
  }
  std::cout << "[SyslinuxSetup] syslinux not found in PATH. Attempting to "
               "install...\n";
  int installReturn = std::system("sudo pacman -Syu install syslinux");
  if (installReturn != 0) {
    std::cerr
        << "[SyslinuxSetup] Failed to install syslinux... try a manual install";
    return false;
  }
  int checkAgain = std::system("which syslinux > /dev/null 2>&1");
  if (checkAgain != 0) {
    std::cerr << "[SyslinuxSetup] syslinux was installed but still not found. "
                 "Installation might have failed.\n";
    return false;
  }
  std::cout << "[SyslinuxSetup] syslinux successfully installed.";

  return true;
}

bool SyslinuxSetup::copySyslinuxBootFiles(
    const std::string bootfilesSourceListFile,
    const std::string &bootfilesOutputDir) {
  /* TODO given the output directory, copy all required syslinux files to that
   * directory. additionally, should have a way of choosing the required files.
   * Maybe string list from a listings file.syslinux.txt
   */
  // Open the file containing the list of boot files required to load the menu.
  std::ifstream inFile(bootfilesSourceListFile);
  if (!inFile.is_open()) {
    std::cerr << "[SyslinuxSetup] Failed to open bootfiles source list: "
              << bootfilesSourceListFile << " \n";
    return false;
  }

  // Ensure the output directory exists (create if it doesnt)
  try {
    fs::create_directories(bootfilesOutputDir);
  } catch (const std::exception &e) {
    std::cerr << "[SyslinuxSetup] Error creating directories: " << e.what()
              << "\n";
    return false;
  }
  // Read each line from source file list (ignore lines starting with # or empty
  // space)
  std::string fileLine;

  while (std::getline(inFile, fileLine)) {
    if (fileLine.empty() || fileLine[0] == '#') {
      continue;
    }
    // Copy parsed file into bootfilesOutputDir
    fs::path srcPath = fileLine;
    fs::path destPath = fs::path(bootfilesOutputDir) / srcPath.filename();
    // attempt copy
    try {
      fs::copy_file(srcPath, destPath, fs::copy_options::overwrite_existing);
      std::cout << "[SyslinuxSetup] Copied " << srcPath << " -> " << destPath
                << "\n";
    } catch (const std::exception &e) {
      std::cerr << "[SyslinuxSetup] Failed to copy " << srcPath << " to "
                << destPath << ". Error: " << e.what() << "\n";
      return false;
    }
  }
  std::cout << "[SyslinuxSetup] Finished copying Syslinux bootfiles to "
            << bootfilesOutputDir << "\n";
  return true;
}
