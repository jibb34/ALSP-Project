#include "ConfigLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

bool ConfigLoader::loadConfig(const std::string &path) {
  /*
   * TODO:: Implement function:
   * Open config file
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
  return false;
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
