#pragma once

#include <map>
#include <string>
#include <vector>

class ConfigLoader {
public:
  ConfigLoader() = default;

  // load the configuration file specified by path string.
  bool loadConfig(const std::string &path);

  // gets value for "section.key" as a string (if exists), otherwise returns
  // empty string
  std::string getValue(const std::string &section, const std::string &key,
                       const std::string &defValue = "") const;

  // handle config keyvalue pairs with multiple items (key=value1,value2)
  std::vector<std::string> getArrayValue(const std::string &section,
                                         const std::string &key) const;

private:
  // "Section.key" -> string value
  std::map<std::string, std::string> kvStore;

  // Section.key -> vector<string>
  std::map<std::string, std::vector<std::string>> kvStoreArrays;
};
