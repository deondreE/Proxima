#pragma once
#include <string>

namespace UI {

struct AppConfig {
  std::string window_title = "Proxima MVP";
  int initial_width = 640;
  int initial_height = 480;
  std::string deafult_font_path = "./fonts/Delius-Regular.ttf";
};

class ConfigManager {
  AppConfig app_config;

 public:
  bool load(const std::string& config_path);
  const AppConfig& getAppConfig() const { return app_config; }
};

}  // Namespace UI
