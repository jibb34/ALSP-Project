#include "ExtractRocky.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool ExtractRocky::extractAll(const std::string &isoPath,
                              const std::string &workingDir,
                              const std::string &tftpDir,
                              const std::string &httpDir) {
  /*TODO: similar to others, except with custom logic for extracting required
   * files, and maybe downloading other necessary files via wget
   */
  std::cout << "[ExtractRocky] Starting Extraction of " << isoPath << std::endl;

  // create working directory
  if (!fs::exists(workingDir)) {
    if (!fs::create_directories(workingDir)) {
      std::cerr << "[ExtractRocky] Failed to create working directory: "
                << workingDir << std::endl;
      return false;
    }
  }
  /* TODO: Extract iso into workingDir
   * then define expected paths for both kernel and initramfs/initrd
   * being workingDir/images/pxeboot/vmlinuz or /images/pxeboot/initrd.img
   * copy kernel and initrd files under a rocky subdirectory
   * in tftp server... tftp::/rocky/vmlinuz, tftp::/rocky/initrd.img
   * then copy the rest of the extracted files to the http directory.
   */
  // extract iso
  std::string extractCmd = "bsdtar -xf" + isoPath + " -C " + workingDir;
  int ret = std::system(extractCmd.c_str());
  if (ret != 0) {
    std::cerr << "[ExtractRocky] Failed to extract ISO with bsdtar... is it "
                 "installed?";
    return false;
  }
  // define paths for kernel and initrd
  fs::path kernelSrc = fs::path(workingDir) / "images/pxeboot/vmlinuz";
  fs::path initrdSrc = fs::path(workingDir) / "images/pxeboot/initrd.img";
  // TODO: Implement grep response to see if the file exists
  if (!fs::exists(kernelSrc)) {
    std::cerr << "[ExtractRocky] Kernel not found at " << kernelSrc
              << std::endl;
    return false;
  }
  if (!fs::exists(initrdSrc)) {
    std::cerr << "[ExtractRocky] Initrd not found at " << initrdSrc
              << std::endl;
    return false;
  }
  // copy kernel and initrd to TFTP directory
  fs::path rockyTftpDir = fs::path(tftpDir) / "rocky";
  fs::create_directories(rockyTftpDir);

  try {
    fs::copy_file(kernelSrc, rockyTftpDir / "vmlinuz",
                  fs::copy_options::overwrite_existing);
    fs::copy_file(initrdSrc, rockyTftpDir / "initrd.img",
                  fs::copy_options::overwrite_existing);
  } catch (const fs::filesystem_error &ex) {
    std::cerr << "[ExtractRocky] Error copying kernel/initrd: " << ex.what()
              << std::endl;
    return false;
  }
  // copy rest of extracted file to http directory
  fs::path rockyHttpDir = fs::path(httpDir) / "rocky";
  fs::create_directories(rockyHttpDir);

  try {
    fs::copy(workingDir, rockyHttpDir,
             fs::copy_options::recursive |
                 fs::copy_options::overwrite_existing);
  } catch (const fs::filesystem_error &ex) {
    std::cerr << "[ExtractRocky] Error copying files to HTTP directory: "
              << ex.what() << std::endl;
    return false;
  }
  std::cout
      << "[ExtractRocky] Extraction and file copying completed successfully."
      << std::endl;
  return true;
}
