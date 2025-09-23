#include "UI/ConfigManager.hpp"
#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>

namespace UI {

bool ConfigManager::load(const std::string& config_path) {
  try {
    YAML::Node config = YAML::LoadFile(config_path);

    if (config["application"]) {
      auto app_node = config["application"];

      if (app_node["window_title"]) {
        app_config.window_title = app_node["window_title"].as<std::string>();
      }
      if (app_node["initial_width"]) {
        app_config.initial_width = app_node["initial_width"].as<int>();
      }
      if (app_node["initial_height"]) {
        app_config.initial_height = app_node["initial_height"].as<int>();
      }
    } else {
      std::cerr << "Warning: 'application' node not found in " << config_path
                << std::endl;
    }
    return true;
  } catch (const YAML::BadFile& e) {
    std::cerr << "Error loading config file '" << config_path
              << "': " << e.what() << std::endl;
    return false;
  } catch (const YAML::Exception& e) {
    std::cerr << "Error parsing config file '" << config_path
              << "': " << e.what() << std::endl;
    return false;
  } catch (...) {
    std::cerr << "An unexpected error occurred while loading '" << config_path
              << "'" << std::endl;
    return false;
  }
}
}  // namespace UI
