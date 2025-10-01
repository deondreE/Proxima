#pragma once
#include <string>
#include <memory>
#include "Platform.hpp"

namespace Core {
  class IImage;

  class PEXPORT IImageLoader {
    public:
      virtual ~IImageLoader() = default;
      virtual std::unique_ptr<IImage> loadImage(const std::string& path) = 0;
  };
} // Core