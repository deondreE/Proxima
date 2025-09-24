#pragma once
#include "View.hpp"

namespace UI {

class Image : public View {
 private:
  std::string image_path;
  SDL_Texture* texture = nullptr;
  bool texture_needs_update = false;

  void updateTexture(SDL_Renderer* renderer);
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

    void draw(SDL_Renderer* renderer) override;
    bool handleProximaEvent(const ProximaEvent& event) override;

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&&) = delete;
    Image& operator=(Image&&) = delete;
};

}  // Namespace UI