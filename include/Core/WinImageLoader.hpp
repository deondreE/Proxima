#pragma once
#include "IImageLoader.hpp"
#include "WinImage.hpp"
#include <iostream>
#include <string>

#include <memory>
#include "Platform.hpp"

namespace Core {
  class PEXPORT WinImageLoader : public Core::IImageLoader {
    public:
      WinImageLoader() {}

      ~WinImageLoader() {}

      std::unique_ptr<IImage> loadImage(const std::string& path) override {
        if (path.empty()) {
          std::cerr << "WARNING: Attempted to load empty path.\n";
          return nullptr;
        }

        HBITMAP hBitmap = nullptr;
        int width = 0;
        int height = 0;

        hBitmap = (HBITMAP)LoadImageA(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        if (!hBitmap) {
          DWORD lastError = GetLastError();
          std::cerr << "ERROR: Faield to load image '" << path << "'. WinError: " << lastError << "\
          n";
          return nullptr;
        }

        BITMAP bm;
        if (GetObject(hBitmap, sizeof(BITMAP), &bm)) {
          width = bm.bmWidth;
          height = bm.bmHeight;
        } else {
          std::cerr << "WARNING: Could not get dimentions of bitmap\n";
          DeleteObject(hBitmap);
          return nullptr;
        }

        return std::make_unique<WinImage>(hBitmap, width, height);
      }
  };
}
