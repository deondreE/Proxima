#pragma once
#include "View.hpp"
#include "Core/IImage.hpp"
#include "Core/IImageLoader.hpp"
#include "Platform.hpp"
#include <string>
#include <memory>

namespace UI {

class PEXPORT Image : public View {
 private:
  std::string image_path;
  std::unique_ptr<Core::IImage> loaded_image = nullptr;
  bool image_needs_reload = false;

 public:
    Image(const std::string& path = "", int x = 0, int y = 0, int w = 0, int h = 0);
    
    ~Image();

    Image& size(int w, int h) {
        View::size(w, h);
        return *this;
    }
    Image& pos(int nx, int ny) {
        View::pos(nx, ny);
        return *this;
    }
    Image& z_index(int z) {
        View::z_index(z);
        return *this;
    }

    Image& setImagePath(const std::string& path);

    void setContext(const ViewContext& context) override;
    void draw(const ViewContext& context) override;
    bool handleProximaEvent(const ProximaEvent& event) override;

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&&) = delete;
    Image& operator=(Image&&) = delete;
};

}  // Namespace UI