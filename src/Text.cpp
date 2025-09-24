#include "UI/Text.hpp"
#include <iostream>

namespace UI {

Text::Text(const std::string& text_str, TTF_Font* initialFont, SDL_Color color) : 
text_content(text_str),
font(initialFont),
text_color(color),
text_texture(nullptr),
texture_needs_update(false),
word_wrap(false) {

}

Text::~Text()  {
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }
}

void Text::updateTexture(SDL_Renderer* renderer) {
    if (!texture_needs_update || !font || text_content.empty()) {
        return;
    }

    if (text_texture) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }

    SDL_Surface* surface = nullptr;
    if (word_wrap) {
        if (width > 0) {
            surface = TTF_RenderText_Blended_Wrapped(font, text_content.c_str(), text_content.length(), text_color, width);
        } else {
            surface = TTF_RenderText_Blended(font, text_content.c_str(), text_content.length(), text_color);
        }
    } else {
        surface = TTF_RenderText_Blended(font, text_content.c_str(), text_content.length(), text_color);
    }

    if (!surface) {
        std::cerr << "Text::updateTexture:: Failed to render text surface: " << std::endl;
        return;
    }

    text_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!text_texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
    }

    SDL_DestroySurface(surface);
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

Text& Text::setWordWrap(bool wrap) {
    if (word_wrap != wrap) {
        word_wrap = wrap;
        texture_needs_update = true; 
    }
    return *this;
}

void Text::draw(SDL_Renderer* renderer) {
    updateTexture(renderer);

    if (text_texture) {
        float tex_w, tex_h;
        SDL_GetTextureSize(text_texture, &tex_w, &tex_h);
        SDL_FRect dest_rect = {(float)x, (float)y, tex_w, tex_h};
        SDL_RenderTexture(renderer, text_texture, NULL, &dest_rect);
    }

    View::draw(renderer);
}

void Text::handleEvent(const SDL_Event& event) {
    View::handleEvent(event);
}

} // Namespace UI