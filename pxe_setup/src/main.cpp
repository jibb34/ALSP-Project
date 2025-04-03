// main.cpp
#include "ConfigLoader.hpp"
#include "IsoManager.hpp"
#include "SyslinuxSetup.hpp"
#include <chrono>
#include <iostream>
#include <string>

// Optional function to show usage instructions
void printUsage(const std::string &programName) {
  std::cout << "Usage: " << programName << " [options]\n"
            << "Options:\n"
            << "  -h, --help   Show this help message.\n"
            << std::endl;
}

int main(int argc, char *argv[]) {
  // Basic argument handling example
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "-h" || arg == "--help") {
      printUsage(argv[0]);
      return 0;
    } else {
      std::cout << "Unknown option: " << arg << "\n\n";
      printUsage(argv[0]);
      return 1;
    }
  }
  std::cout << "Hello world" << std::endl;
  SyslinuxSetup setup;
  ConfigLoader cl;
  std::string srcListFile =
      "/home/jackjibb/Auto-Linux_System_Provisioning/config/syslinux.txt";
  std::string outputDir = "/home/jackjibb/Auto-Linux_System_Provisioning";
  std::string configFile =
      "/home/jackjibb/Auto-Linux_System_Provisioning/config/bootfiles.conf";

  // bool ok = setup.copySyslinuxBootFiles(srcListFile, outputDir);
  // test config works ok
  bool okConfig = cl.loadConfig(configFile);
  if (!okConfig) {
    std::cerr << "[Main] Error Loading Configuration File\n";
    return 1;
  }
  // test individual value
  std::cout << cl.getValue("arch", "iso_name") << std::endl;
  // test array
  std::vector<std::string> valueVector =
      cl.getArrayValue("rocky", "archives_needed");

  for (const auto &val : valueVector) {
    std::cout << val << std::endl;
  }
  // test iso download
  IsoManager manager;
  std::string section;
  bool ok;
  std::string isoDir = "/home/jackjibb/Auto-Linux_System_Provisioning/iso";
  manager.checkAndDownloadIso(cl, "rocky", isoDir);

  // Main program logic goes here
  // *********************************************
  /* TODO: Implement sequential calls to each function.
   * Step 1: SyslinuxSetup.cpp: Verify installation of syslinux and copy
   *  required files
   * Step 2: ConfigLoader.cpp: Parse config file into global dictionary
   * Step 3: IsoManager.cpp: Check if ISO files specified in config
   *  dict exist in their correct location, if not, download them. Then
   * extract the required files for boot. Step 3b: Extract iso file: >
   * ExtractBase.hpp: template for future extraction functions (for adding
   * new Flavours of OS) ExtractArch.cpp/ExtractRocky.cpp/ExtractUbuntu.cpp:
   * Specific Extraction functions Step 4: PxelinuxConfigGenerator.cpp:
   * Generate the pxelinux.cfg/default file to serve to the PXE boot
   * process.
   */

  //********************************************
  return 0; // Indicate success
}
