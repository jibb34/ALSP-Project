#pragma once
#include <string>

class ConfigLoader;

class IsoManager {
public:
  /* Function to check if an ISO exists in the directory, and if not, download
   * it from the provided link.
   */
  bool checkAndDownloadIso(const ConfigLoader &config,
                           const std::string &section,
                           const std::string &isoDir);
};
