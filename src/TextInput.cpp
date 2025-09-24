#include "UI/TextInput.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <iostream>

namespace UI {
TextInput::TextInput() {
  texture_needs_update = true;
}

TextInput::~TextInput() {
  if (text_texture) {
    SDL_DestroyTexture(text_texture);
    text_texture = nullptr;
  }

  if (font) {
    TTF_CloseFont(font);
    font = nullptr;
  }
}

void TextInput::updateTexture(SDL_Renderer* renderer) {
  if (!texture_needs_update || !font || textBuffer.empty()) {
    return;
  }

  if (text_texture) {
    SDL_DestroyTexture(text_texture);
    text_texture = nullptr;
  }

  SDL_Surface* surface = TTF_RenderText_Blended(font, textBuffer.c_str(),
                                                textBuffer.length(), textColor);

  if (!surface) {
    std::cerr << "Text::updateTexture:: Failed to render text surface: "
              << std::endl;
    return;
  }

  text_texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!text_texture) {
    std::cerr << "Failed to create text texture: " << SDL_GetError()
              << std::endl;
  }

  SDL_DestroySurface(surface);
  texture_needs_update = false;
}

bool TextInput::reopenFont() {
  if (font) {
    TTF_CloseFont(font);
    font = nullptr;
  }

  if (!font_path.empty() && font_size > 0) {
    font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
      std::cerr << "Text::reopenFont: Failed to open font '" << font_path
                << "' with size " << font_size << "! TTF_Error: " << std::endl;
      return false;
    }
  }
  texture_needs_update = true;

  return true;
}

TextInput& TextInput::text(const std::string& str) {
  textBuffer = str;
  cursorPosition = textBuffer.length();
  return *this;
}

TextInput& TextInput::setFont(const std::string& fontPath, int size) {
  if (font_path != fontPath || font_size != size) {
    font_path = fontPath;
    font_size = size;
    reopenFont();
  }
  return *this;
}

TextInput& TextInput::setColor(SDL_Color clr) {
  textColor = clr;
  return *this;
}

TextInput& TextInput::setCursorColor(SDL_Color clr) {
  cursorColor = clr;
  return *this;
}

TextInput& TextInput::setBackground(SDL_Color clr) {
  backgroundColor = clr;
  return *this;
}

void TextInput::draw(SDL_Renderer* renderer) {
    if (!font) {
        std::cerr << "TextInput: Font not set, cannot render." << std::endl;
        return;
    }

    // bg
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.b, backgroundColor.g, backgroundColor.a);
    SDL_FRect bgRect = {(float)x, (float)y, (float)width, (float)height}; 
    SDL_RenderFillRect(renderer, &bgRect);

    if (texture_needs_update) {
        updateTexture(renderer);
    }
    if (text_texture) {
        float tex_w, tex_h;
        SDL_GetTextureSize(text_texture, &tex_w, &tex_h);
        SDL_FRect dest_rect = {(float)x, (float)y, tex_w, tex_h};
         if (dest_rect.w > width) {
            dest_rect.w = width;
        }
        SDL_RenderTexture(renderer, text_texture, NULL, &dest_rect);
    }

    if (isFocused && cursorVisible) {
        int cursorDrawX = x;
        if (!textBuffer.empty()) {
            int avgCharWidth = std::max(1, width / std::max(1, (int)textBuffer.length())); // Rough guess for average char width
            cursorDrawX = x + std::min(cursorPosition * avgCharWidth, width - 20); // Clamp within field
            if (cursorPosition == 0) cursorDrawX = x; // At the start if cursor is at beginning
            else if (cursorPosition == textBuffer.length()) cursorDrawX = x + static_cast<int>(textBuffer.length() * avgCharWidth); // At the end
            else cursorDrawX = x + static_cast<int>(cursorPosition * avgCharWidth); // Mid-string
            // Clamp cursor position to be visible within the field
            cursorDrawX = std::max(x, std::min(cursorDrawX, x + width - 2));
        }

        SDL_SetRenderDrawColor(renderer, cursorColor.r, cursorColor.g, cursorColor.b, cursorColor.a);
        SDL_FRect cursorRect = {(float)cursorDrawX, (float)y + 5.0f, 2.0f, (float)height - 10.0f}; // Simple cursor line
        SDL_RenderFillRect(renderer, &cursorRect);
    }

    View::draw(renderer);
}

void TextInput::requestRedraw() {
    dirty = true;
    texture_needs_update = true;
}

void TextInput::handleEvent(const SDL_Event& event) {
    if (!isFocused) {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.button.x >= x && event.button.x <= x + width &&
                event.button.y >= y && event.button.y <= y + height) {
                setFocus(true);
                cursorVisible = true;
                lastBlinkTime = SDL_GetTicks();
                }
        }
        return;
    }

    if (event.type == SDL_EVENT_TEXT_INPUT) {
        insertCharacter(event.text.text);
        std::cout << event.text.text;
    }
     else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (!(event.button.x >= x && event.button.x <= x + width &&
              event.button.y >= y && event.button.y <= y + height)) {
            setFocus(false); // Lose focus if clicked outside
        } else {
            // Clicked inside: Reposition cursor based on X coordinate.
            // This requires measuring text width up to the click position.
            // For MVP, we'll just ensure focus remains and cursor is visible.
            cursorVisible = true;
            lastBlinkTime = SDL_GetTicks();
        }
    }
    cursorVisible = true;
    lastBlinkTime = SDL_GetTicks();
}

void TextInput::insertCharacter(const char* text_utf8) {
    if (!text_utf8 || *text_utf8 == '\0' || !isFocused) return;

    // Clamp cursor position before insertion
    if (cursorPosition < 0) cursorPosition = 0;
    if (cursorPosition > textBuffer.length()) cursorPosition = textBuffer.length();
    
    textBuffer.insert(cursorPosition, text_utf8);
    cursorPosition += strlen(text_utf8);
    texture_needs_update = true;
    cursorVisible = true;
    lastBlinkTime = SDL_GetTicks();

    texture_needs_update = true;
}

void TextInput::deleteCharacter() {
    if (!isFocused) return;
    if (cursorPosition > 0) {
        textBuffer.erase(cursorPosition - 1, 1);
        cursorPosition--;
        texture_needs_update = true;
        cursorVisible = true;
        lastBlinkTime = SDL_GetTicks();
    }
}

void TextInput::moveCursor(int delta) {
    if (!isFocused) return;
    cursorPosition += delta;
    if (cursorPosition < 0) cursorPosition = 0;
    if (cursorPosition > textBuffer.length()) cursorPosition = textBuffer.length();
    
    cursorVisible = true; 
    lastBlinkTime = SDL_GetTicks();
}

void TextInput::updateCursorBlink(Uint32 currentTimeMillis) {
    if (!isFocused) {
        cursorVisible = false; 
        return;
    }
    if (currentTimeMillis - lastBlinkTime > 0) {
        cursorVisible = !cursorVisible;
        lastBlinkTime = currentTimeMillis;
        requestRedraw(); // Signal redraw if cursor state changes
    }
}

void TextInput::setFocus(bool focus) {
    isFocused = focus;
    cursorVisible = focus; // Show cursor when focused, hide when unfocused
    lastBlinkTime = SDL_GetTicks(); // Reset blink timer on focus change
    requestRedraw();
}

}  // Namespace UI