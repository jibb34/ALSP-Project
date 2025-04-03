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

private:
  // set the download as a background process
  bool forkDownloadIso(const std::string &cmd);
};
