#include "UI/Text.hpp"
#include <iostream>

namespace UI {

bool Text::reopenFont() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (!font_path.empty() && font_size > 0) {
        font = TTF_OpenFont(font_path.c_str(), font_size);
        if (!font) {
             std::cerr << "Text::reopenFont: Failed to open font '" << font_path
                      << "' with size " << font_size << "! TTF_Error: " << std::endl;
                      return  false;
        }
    }
    texture_needs_update = true;

    return  true;
}

Text::Text(const std::string& text, const std::string& initial_font_path,
           int initial_font_size, SDL_Color color)
    : text_content(text),
      font(nullptr), // Initialize font to nullptr
      font_path(initial_font_path), // Store path
      font_size(initial_font_size), // Store size
      text_color(color),
      text_texture(nullptr),
      texture_needs_update(true),
      word_wrap(false) {
    reopenFont();
}

Text::~Text()  {
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
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

Text& Text::setFont(const std::string& fontPath, int size) {
    if (font_path != fontPath || font_size != size) {
        font_path = fontPath;
        font_size = size;
        reopenFont();
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