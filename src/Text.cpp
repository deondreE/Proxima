#include "UI/Text.hpp"
#include <iostream>

namespace UI {

Text::Text(const std::string& text_str, TTF_Font* initialFont, SDL_Color color) : 
text_content(text_str),
font(initialFont),
text_color(color),
text_texture(nullptr),
texture_needs_update(false) {

}

Text::~Text()  {
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }
}

void Text::updateTexture(SDL_Renderer* renderer) {
    if (!renderer || !font || text_content.empty()) {
        if (text_texture) {
            SDL_DestroyTexture(text_texture);
            text_texture = nullptr;
        }
        width = 0; height = 0;
        texture_needs_update = false;
        return;
    }

    if (!texture_needs_update) return;

    if (text_texture ) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }

    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text_content.c_str(), text_content.length(), text_color);
    if (!text_surface) {
        std::cerr << "Text::updateTexture:: Failed to render text surface: " << std::endl;
        width = 0; height = 0;
        texture_needs_update = false;
        return;
    }

    width = text_surface->w;
    height = text_surface->h;

    SDL_DestroySurface(text_surface);
    texture_needs_update = false;
}

Text& Text::content(const std::string& str) {
    if (text_content != str) {
        text_content = str;
        texture_needs_update = true;
    }
    return *this;
}

Text& Text::setFont(TTF_Font* newFont) {
    if (font != newFont) {
        font = newFont;
        texture_needs_update = true;
    }
    return *this;
}

Text& Text::setColor(SDL_Color newColor) {
    if (text_color.r != newColor.r || text_color.g != newColor.g ||
        text_color.b != newColor.b || text_color.a != newColor.a) {
            text_color = newColor;
            texture_needs_update = true;
        }
    return *this;
}

void Text::draw(SDL_Renderer* renderer) {
    updateTexture(renderer);

    if (text_texture) {
        SDL_FRect dest_rect = {(float)x, (float)y, (float)width, (float)height};
        SDL_RenderTexture(renderer, text_texture, NULL, &dest_rect);
    }

    View::draw(renderer);
}

void Text::handleEvent(const SDL_Event& event) {
    View::handleEvent(event);
}

} // Namespace UI