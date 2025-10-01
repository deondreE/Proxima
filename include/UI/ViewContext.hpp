#pragma once
#include "Platform.hpp"

namespace UI {
    class Renderer;
}

struct PEXPORT ViewContext {
    UI::Renderer* renderer = nullptr;
};