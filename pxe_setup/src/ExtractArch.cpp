#include "ExtractArch.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool ExtractArch::extractAll(const std::string &isoPath,
                             const std::string &workingDir,
                             const std::string &tftpdir,
                             const std::string &httpDir) {
  /* TODO: Mount or extract ISO... this can be done in many ways
   * ============================================================
   * locate kernel and initramfs within extracted content
   * ============================================================
   * Copy kernel and initramfs to tftp root directory
   * (containers/tftp/bootfiles)
   * ============================================================
   * Copy everything else to httpDir possibly with rsync? or other copy
   * (../../containers/http/arch)
   */
  return false;
}
/* TODO: Maybe implement alternative functions that are required for arch
 * specifically, based on further research
 */
