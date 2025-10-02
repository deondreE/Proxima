#include "Core/ConfigManager.hpp"
#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>

namespace Core {

template<typename T>
T ConfigManager::get_value_or_default(const YAML::Node& node, const std::string& key, const T& default_value) const {
  if (node && node[key]) {
    try {
      return node[key].as<T>();
    } catch (const YAML::BadConversion& e) {
      std::cerr << "Warning: Could not convert '" << key
                << "' to expected type. Using default. Error: " << e.what()
                << std::endl;
      return default_value;
    }
  }

  return default_value;
}

template <typename T, size_t N>
std::array<T, N> ConfigManager::get_array_or_default(const YAML::Node& node, const std::string& key, const std::array<T, N>& default_value) const {
  if (node && node[key]) {
    const YAML::Node& array_node = node[key];
    if (array_node.IsSequence() && array_node.size() == N) {
      std::array<T, N> result;
      for (size_t i = 0; i < N; ++i) {
        try {
          result[i] = array_node[i].as<T>();
        } catch(...) {
          std::cerr << "Something broke\n";
          return default_value;
        }
      }
      return result;
    } else {
      std::cerr << "WARNING: Node '" << key << "' is not valid\n";
    }
  }
  return default_value;
}

bool ConfigManager::load(const std::string& config_path) {
  try  {
    YAML::Node config = YAML::LoadFile(config_path);

    if (config["application"]) {
      const YAML::Node& app_node = config["application"];
      app_config.window_title = get_value_or_default<std::string>(
        app_node, "window_title", app_config.window_title);
      app_config.initial_height = get_value_or_default<int>(
        app_node, "height", app_config.initial_height);
      app_config.initial_width = get_value_or_default<int>(
        app_node, "width", app_config.initial_width);
      app_config.icon_path = get_value_or_default<std::string>(
        app_node, "icon", app_config.icon_path);
    } else if (config["colorscheme"]) {
      const YAML::Node& colorscheme_node = config["colorscheme"];
      color_config.core_bg = get_array_or_default<unsigned char, 4>(colorscheme_node, "core_bg", color_config.core_bg);
    }
    else if (config["fonts"]) {
      const YAML::Node& font_node = config["fonts"];
      font_config.header_font = get_value_or_default<std::string>(font_node, "header_font", font_config.header_font);
    }
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
