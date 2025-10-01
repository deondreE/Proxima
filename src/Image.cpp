#include "UI/Image.hpp"
#include <cassert>
#include <iostream>
#include <filesystem>

namespace UI {

Image::Image(const std::string& path, int x, int y, int w, int h) 
    : image_path(path)
{
    View::pos(x, y);
    View::size(w, h);
    if (!path.empty()) {
       image_needs_reload = true;
    }
}

Image::~Image() {
}

void Image::setContext(const ViewContext& context) {
    View::setContext(context);

    if (_context.renderer && image_needs_reload && !image_path.empty()) {
        std::filesystem::path currentPath = std::filesystem::current_path();
        currentPath.make_preferred();

        std::filesystem::path full_path = currentPath / image_path;
        std::filesystem::path normalized_path = full_path.lexically_normal();
        normalized_path.make_preferred();
        
       auto test = _context.renderer->getImageLoader();
       std::cout << image_path << std::endl;
       loaded_image = test->loadImage(normalized_path.string());
       if (!loaded_image) {
        std::cerr << "ERROR: Failed to load image through loader \n";
       } else {
        if (width == 0 && height == 0) {
            View::size(loaded_image->getWidth(), loaded_image->getHeight());
        } else if (width == 0) {
            View::size(static_cast<int>(height * (static_cast<float>(loaded_image->getWidth()) / loaded_image->getWidth())), height);
        } else if (height == 0) {
           View::size(width, static_cast<int>(width * (static_cast<float>(loaded_image->getHeight()) / loaded_image->getWidth())));
        }
       }
       image_needs_reload = false;
    } else if (image_path.empty()) {
        loaded_image = nullptr;
        image_needs_reload = false;
    }
}

Image& Image::setImagePath(const std::string& path) {
    if (image_path != path) {
        image_path = path;
        image_needs_reload = true;
    }
    return *this;
}

void Image::draw(const ViewContext& context) {
    Renderer* renderer = context.renderer;
    assert(renderer && "Renderer must be valid in ViewContext!");
    assert(_context.renderer == renderer && "Context mismatch in Image::draw");

    if (!loaded_image || width <= 0 || height <= 0) {
        View::draw(context);
        return;
    }

    int drawX = getAbsoluteX(); 
    int drawY = getAbsoluteY();
    renderer->drawImage(loaded_image.get(), 0, 0, loaded_image->getWidth(), loaded_image->getHeight(), drawX, drawY, width, height);

    View::draw(context);
}

bool Image::handleProximaEvent(const ProximaEvent& event) {
    return View::handleProximaEvent(event);
}

} // Namespace UI