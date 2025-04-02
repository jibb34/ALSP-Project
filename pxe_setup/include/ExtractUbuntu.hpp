

#pragma once

#include "ExtractOSTemplate.hpp"

class ExtractUbuntu : public ExtractOSTemplate {
public:
  bool extractAll(const std::string &isoPath, const std::string &workingDir,
                  const std::string &tftpDir,
                  const std::string &httpDir) override;
};
