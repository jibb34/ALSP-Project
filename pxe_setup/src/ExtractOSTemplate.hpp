#pragma once

#include <string>

class ExtractBase {
public:
  virtual ~ExtractBase() = default;

  virtual bool extractAll(const std::string &isoPath,
                          const std::string &workingDir, std::string &tftpDir,
                          const std::string &httpDir) = 0;
  /* isoPath: path to OS iso file (/{rootdir}/isos/rocky-9.5.iso for example)
   * workingDir: Path to a temp directory to mount/extract files
   * tftpDir: directory where kernal + initramfs files will be sent
   * httpDir: directory for rest of required boot files
   * NOTE: returns true on success
   */
};
