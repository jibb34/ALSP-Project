#include "IsoManager.hpp"
#include "ConfigLoader.hpp"
#include <cstdlib> // for std::system
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool IsoManager::checkAndDownloadIso(const ConfigLoader &config,
                                     const std::string &section,
                                     const std::string &isoDir) {
  /* TODO: Implement Function:
   * iso name and iso_location are given in the config file like such:
   *
   * [arch]
   * iso_url=https://geo.mirror.pkgbuild.com/iso/latest/
   * iso_name=archlinux.iso
   *
   *
   * get the config value of the iso_url and iso_name,
   * ===============================================================
   * then sanitise the iso url, making sure it is in the right format,
   * i.e. if the user enters with a trailing "/", it should remove it
   * ===============================================================
   * check if the isoDir exists, if not, create it,
   * with fs::path(isoDir) / isoName
   * if isoDir exists, the isoPath exists as well, skip download
   * ===============================================================
   * Download file if doesnt exist via any download method (curl wget, libcurl,
   * etc)
   */
  return false;
}
