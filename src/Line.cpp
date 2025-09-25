#include "UI/Line.hpp"
#include <iostream>

namespace UI {
Line::Line(int x1, int y1, int x2, int y2, SDL_Color c, int t) 
    : x1_rel(x1), y1_rel(y1), x2_rel(x2), y2_rel(y2), color(c), thickness(t)
{
    int minX = std::min(x1_rel, x2_rel);
    int minY = std::min(y1_rel, y2_rel);
    int maxX = std::max(x1_rel, x2_rel);
    int maxY = std::max(y1_rel, y2_rel);


    View::pos(minX, minY);
    View::size(maxX -  minX + thickness, maxY - minY + thickness);
}

Line& Line::setPoints(int x1, int y1, int x2, int y2)
{
    x1_rel = x1;
    x2_rel = x2;
    y1_rel = y1;
    y2_rel = y2;

    int minX = std::min(x1_rel, x2_rel);
    int minY = std::min(y1_rel, y2_rel);
    int maxX = std::max(x1_rel, x2_rel);
    int maxY = std::max(y1_rel, y2_rel);
    View::size(maxX -  minX + thickness, maxY - minY + thickness);
    return *this;
}

Line& Line::setColor(SDL_Color c) {
    color = c;
    return *this;
}

Line& Line::setThickness(int t) {
    thickness = std::max(1, t);
    return *this;
}

void Line::draw(SDL_Renderer* renderer) {
    SDL_Color originalColor;
    SDL_GetRenderDrawColor(renderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    float abs_x1 = (float)x + x1_rel;
    float abs_y1 = (float)y + y1_rel;
    float abs_x2 = (float)x + x2_rel;
    float abs_y2 = (float)y + y2_rel;

    if (thickness <= 1) {
        SDL_RenderLine(renderer, abs_x1, abs_y1, abs_x2, abs_y2);
    } else {
        float dx = abs_x2 - abs_x1;
        float dy = abs_y2 - abs_y1;
        float len = SDL_sqrtf(dx*dx + dy*dy);


        SDL_FRect dot = {abs_x1-thickness/2.0f, abs_y1 - thickness/2.0f, (float)thickness, (float)thickness};
        SDL_RenderFillRect(renderer, &dot);
    }

    SDL_SetRenderDrawColor(renderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
    View::draw(renderer);
}

bool Line::handleProximaEvent(const ProximaEvent& event) {
    return View::handleProximaEvent(event);
}

} // namespace UI