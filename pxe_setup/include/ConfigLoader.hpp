#pragma once

#include <map>
#include <string>

class ConfigLoader {
public:
  // load the configuration file specified by path string.
  bool loadConfig(const std::string &path);

  // get all configuration key-value pairs and store them into a global
  // dictionary
  std::string getValue(const std::string &section, const std::string &key,
                       const std::string &defValue = "") const;
};
