#pragma once
#include <string>
#include <vector>
#include <array>
#include "UI/Color.hpp"
#include "TextRenderer.hpp"
#include <yaml-cpp/yaml.h>
#include "Platform.hpp"

namespace Core {
typedef struct ColorschemeConfig {
  std::array<unsigned char, 4> core_bg = {50, 50, 50, 255};
} ColorschemeConfig;

typedef struct FontConfig {
  std::string header_font;
} FontConfig;

struct AppConfig {
  std::string window_title = "Proxima MVP";
  int initial_width = 640;
  int initial_height = 480;
  std::string icon_path = "1";
};

class PEXPORT ConfigManager {
  template<typename T>
  T get_value_or_default(const YAML::Node& node, const std::string& key, const T& default_value) const;
  template<typename T, size_t N>
  std::array<T, N> get_array_or_default(const YAML::Node& node, const std::string& key, const std::array<T, N>& default_value) const;
  AppConfig app_config;
  ColorschemeConfig color_config;
  FontConfig font_config;

 public:
  bool load(const std::string& config_path);
  const AppConfig& getAppConfig() const { return app_config; }
};

}  // Namespace Core
