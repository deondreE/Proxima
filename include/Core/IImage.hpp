#pragma once
#include "Platform.hpp"
namespace Core {
class PEXPORT IImage {
public:
  virtual ~IImage() = default;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
};
}
