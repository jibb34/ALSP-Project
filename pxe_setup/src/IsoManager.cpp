#include "IsoManager.hpp"
#include "ConfigLoader.hpp"
#include <cstdlib> // for std::system
#include <filesystem>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

bool IsoManager::forkDownloadIso(const std::string &cmd) {
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "[IsoManagerFork] fork() failed\n";
    return false;
  }
  if (pid == 0) {
    // child process: perform download
    int ret = std::system(cmd.c_str());
    // exit with download return status
    exit(ret);
  } else {
    // parent process
    int status;
    if (waitpid(pid, &status, 0) == -1) {
      std::cerr << "[IsoManagerFork] waitpid() failed" << std::endl;
      return false;
    }
    if (WIFEXITED(status)) {
      int exitStatus = WEXITSTATUS(status);
      if (exitStatus == 0) {
        std::cout << "[IsoManagerFork] Download Completed Successfully.\n";
        return true;
      } else {
        std::cerr << "Download failed with exit status: " << exitStatus
                  << std::endl;
        return false;
      }
    }
  }
  return false;
}
bool IsoManager::checkAndDownloadIso(const ConfigLoader &config,
                                     const std::string &section,
                                     const std::string &isoDir) {

  // get config value of iso_url and name
  std::string isoUrl = config.getValue(section, "iso_url");
  std::string isoName = config.getValue(section, "iso_name");

  // check is not empty
  if (isoUrl.empty() || isoName.empty()) {
    std::cerr << "[IsoManager] Error: Missing value in iso_url or iso_name for "
                 "section: "
              << section << ".\n";
    return false;
  }
  // Make sure the the isoDir exists
  fs::create_directories(isoDir);
  // Create the full file path
  fs::path isoPath = fs::path(isoDir) / isoName;
  // Check if iso already exists.
  if (fs::exists(isoPath)) {
    std::cout << "[IsoManager] ISO already exists at: " << isoUrl
              << "\nSkipping download...\n";
    return true;
  }
  std::cout << "[IsoManager] Downloading ISO from " << isoUrl << " to "
            << isoPath << "\n";
  // perform download through cURL
  std::string cmd = "curl -L -o " + isoPath.string() + " " + isoUrl + isoName;
  std::cout << cmd << "\n";
  if (!forkDownloadIso(cmd)) {
    std::cerr << "[IsoManager] Download did not complete\n";
    return false;
  }
  // download completed successfully
  std::cout << "[IsoManager] Download completed successfully\n";
  return true;
}
