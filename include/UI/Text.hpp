#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <string>
#include "View.hpp"

namespace UI {

class Text :  public View
{
private:
    std::string text_content;
    TTF_Font* font;
    std::string font_path;
    int font_size;
    SDL_Color text_color;

    SDL_Texture* text_texture;
    bool texture_needs_update; 

    bool word_wrap;

    void updateTexture(SDL_Renderer* renderer);
    bool reopenFont();
public:
    Text(const std::string& text = "", const std::string& initial_font_path = "",
       int initial_font_size = 24, SDL_Color color = {0, 0, 0, 255});

    ~Text() override;

    Text& size(int w, int h) { 
        View::size(w, h);
        texture_needs_update = true;
        return *this;
    }
    Text& pos(int nx, int ny) { View::pos(nx, ny); return *this; }
    Text& content(const std::string& str);
    Text& setWordWrap(bool wrap);
    Text& setFont(const std::string& fontPath, int size);
    Text& setColor(SDL_Color newColor);

    virtual void draw(SDL_Renderer* renderer) override;
    virtual void handleEvent(const SDL_Event& event) override;

    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;
    Text(Text&&) = delete;  
    Text& operator=(Text&&) = delete;
};

} // Namespace UI