#pragma once

#include <string>

class ConfigLoader;

class PxelinuxConfigGenerator {
public:
  // simply generates the pxelinux.cfg/default file
  bool generateDefaultConfig(const ConfigLoader &config);
};
