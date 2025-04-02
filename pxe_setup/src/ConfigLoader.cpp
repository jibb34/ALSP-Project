#include "ConfigLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

bool ConfigLoader::loadConfig(const std::string &path) {
  /*
   * TODO:: Implement function:
   * Open config file
   *
   * -------------------
   * parse file and split into sections via the [os_version] tag
   * Linear parse: as we go down the file line by line, update a Section
   * variable and ignore any empty lines or lines that start with "#"
   * -------------------
   * find key value pair (separated by =)
   * and store in map "Section.Key":"Value"
   * something like: mapkey = currentSection + "." + key
   * map[mapkey] = val;
   * -------------------
   * implement some error parsing, aka a line counter, and checking if a line
   * is in a valid format (key=value). Maybe can introduce variations?
   * such as key=value1,value2 automatically parsing it as an array value
   * -------------------
   *  NOTE: Once config file is loaded (correctly) it should be treated as READ
   * ONLY
   * -------------------
   */

  // open Config file
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "[ConfigLoader] Could not open config file: " << path
              << std::endl;
    return false;
  }
  // keep state of what section we are currently parsing, as well as line and
  // line #. current Section should always start at global.
  std::string currentSection = "global";
  std::string line;
  int lineNumber = 0;
  while (std::getline(file, line)) {
    // update line number
    lineNumber++;

    // trim whitespace lambda function
    auto trim = [](std::string &s) {
      // remove leading space
      while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) {
        s.erase(s.begin());
      }
      // remove trailing space
      while (!s.empty() && (s.back() == ' ' || s.back() == '\t')) {
        s.pop_back();
      }
    };

    trim(line);

    // skip empty lines or lines that start with #
    if (line.empty() || line[0] == '#') {
      continue;
    }
    // If is section line, update the current section
    if (line.front() == '[' && line.back() == ']') {
      currentSection = line.substr(1, line.size() - 2);
      trim(currentSection); // trim in case of whitespace within []
      continue;
    }
    // after handling all special case, now handle default key=value case:

    // find first "="
    size_t eqPos = line.find('=');
    if (eqPos == std::string::npos) {
      // "=" char not found
      std::cerr << "[ConfigLoader] ERROR: Invalid line format at line "
                << lineNumber << ": " << line << std::endl;
      continue;
    }
    // trim sub strings and assign values
    std::string key = line.substr(0, eqPos);
    std::string val = line.substr(eqPos + 1);
    // trim whitespace
    trim(key);
    trim(val);

    if (key.empty()) {
      std::cerr << "[configLoader] Error: empty key at line " << lineNumber
                << ": " << line << std::endl;
      continue;
    }

    // Build map key as "Section.Key"
    std::string mapKey = currentSection + "." + key;

    // parse Comma separated values:
    if (val.find(',') != std::string::npos) {
      std::vector<std::string> tokens;
      {
        // split on comma
        std::stringstream ss(val); // get string stream
        std::string token;
        while (std::getline(ss, token, ',')) {
          trim(token); // for each token in the string, trim whitespace and put
                       // to back of vector
          tokens.push_back(token);
        }
      }
      // Store tokens into the array
      kvStoreArrays[mapKey] = tokens;
      // For convienience, store single string in kvStore
      kvStore[mapKey] = val;

    } else { // if no comma separated values just store as string
      kvStore[mapKey] = val;
    }
  }
  file.close();
  return true;
}

std::string ConfigLoader::getValue(const std::string &section,
                                   const std::string &key,
                                   const std::string &defValue) const {
  /*TODO: Implement Function:
   * from the section and key value, find the subsequent value and assign it to
   * defValue. Purpose of this function should just be to get a value from the
   * config map.
   */
  return "";
}

std::vector<std::string>
ConfigLoader::getArrayValue(const std::string &section,
                            const std::string &key) const {
  // define map key
  std::string mapKey = section + "." + key;

  // set it = the iterator
  auto it = kvStoreArrays.find(mapKey);
  if (it == kvStoreArrays.end()) {
    // return empty vector if not found
    return {};
  }
  // if iterator stops at a kv pair
  // return the value of the value of the pair.
  return it->second;
  ;
}
