#include "UI/Rect.hpp"

namespace UI {

Rect::Rect(int x, int y, int w, int h, const Color& c)
    : color(c) {
    View::pos(x, y);  
    View::size(w, h);
}

Rect& Rect::setColor(const Color& clr) {
    color = clr;
    return *this;
}

void Rect::draw(const ViewContext& context) {
    UI::Renderer* renderer = context.renderer;

    renderer->setDrawColor(color);
    renderer->drawRect(x, y, width, height);
    renderer->fillRect(x,y,width,height);

    View::draw(context);
}
} // Namespace UI