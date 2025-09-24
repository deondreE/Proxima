#include "UI/Image.hpp"
#include <SDL3_image/SDL_image.h> 
#include <iostream>

namespace UI {

Image::Image(const std::string& path, int x, int y, int w, int h) 
    : image_path(path)
{
    View::pos(x, y);
    View::size(w, h);
    if (!path.empty()) {
        texture_needs_update = true;
    }
}

Image::~Image() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Image::updateTexture(SDL_Renderer* renderer) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (!image_path.empty() && renderer) {
        std::cout << image_path;
        texture = IMG_LoadTexture(renderer, image_path.c_str());
        if (!texture) {
            std::cerr << "IMG_LoadTexture Error:" << std::endl;
        } else {
            if (width == 0 && height == 0) {
                // TODO: if 0 get from texture assuming texture is not null.
            }
        }
    }
    texture_needs_update = false;
}

Image& Image::setImagePath(const std::string& path) {
    if (image_path != path) {
         image_path = path;
         texture_needs_update = true;
    }
    return *this;
}

void Image::draw(SDL_Renderer* renderer) {
    if (texture_needs_update) {
        updateTexture(renderer); 
    }

    if (texture) {
        SDL_FRect dstRect = {(float)x, (float)y, (float)width, (float)height};
        SDL_RenderTexture(renderer, texture,nullptr, &dstRect);
    }

    View::draw(renderer);
}

bool Image::handleProximaEvent(const ProximaEvent& event) {
    return View::handleProximaEvent(event);
}

} // Namespace UI