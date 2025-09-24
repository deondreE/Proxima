#include "UI/Rect.hpp"

namespace UI {

Rect::Rect(int x, int y, int w, int h, SDL_Color c)
    : color(c) {
    View::pos(x, y);  
    View::size(w, h);
}

Rect& Rect::setColor(SDL_Color clr) {
    color = clr;
    return *this;
}

void Rect::draw(SDL_Renderer* renderer) {
    // Save original draw color
    SDL_Color originalColor;
    SDL_GetRenderDrawColor(renderer, &originalColor.r, &originalColor.g,
                           &originalColor.b, &originalColor.a);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect rect = {(float)x, (float)y, (float)width, (float)height};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, originalColor.r, originalColor.g,
                           originalColor.b, originalColor.a);

    View::draw(renderer);
}
} // Namespace UI