#pragma once
#include "View.hpp"
#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace UI {

class TextInput : public View {
    std::string textBuffer; 
    int cursorPosition = 0; 
    bool isFocused = false;

    TTF_Font* font;
    SDL_Texture* text_texture;
    std::string font_path;
    bool dirty = false; 
    int font_size;
    bool texture_needs_update;
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Color cursorColor = {0, 0, 0, 255};
    SDL_Color backgroundColor = {255, 255, 255, 255};

    Uint32 lastBlinkTime = 0;
    bool cursorVisible = true; 
    const Uint32 Blink_Interva_MS = 500;

    void updateTexture(SDL_Renderer* renderer);
    bool reopenFont();

public:
    TextInput();
    ~TextInput() override;

    TextInput& text(const std::string& str);
    TextInput& setFont(const std::string& font_path, int font_size);
    TextInput& setColor(SDL_Color clr);
    TextInput& setCursorColor(SDL_Color clr);
    TextInput& setBackground(SDL_Color clr);

    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event) override;

    void setNeedsRedraw(bool val);
    void requestRedraw();
    
    void insertCharacter(const char* text_utf8); // Use text input data
    void deleteCharacter();
    void moveCursor(int delta);
    void updateCursorBlink(Uint32 currentTimeMillis);
    void setFocus(bool focus);
    bool hasFocus() const { return isFocused; }
};

} // Namespace UI