#pragma once
#include "View.hpp"
#include "Color.hpp"
#include "Platform.hpp" 

namespace UI {
    class PEXPORT Line : public View {
        int x1_rel, x2_rel, y1_rel, y2_rel;
        Color color = {0, 0, 0, 255};
        int thickness = 1;

        public:
            Line(int x1 = 0, int y1 = 0, int x2 = 100, int y2 = 0,
                const Color& c = {0, 0, 0, 255}, int t = 1);
            
            ~Line() override = default;

            Line& setPoints(int x1, int y1, int x2, int y2);
            Line& setColor(const Color& c);
            Line& setThickness(int t);

            void draw(Renderer* renderer) override;

            bool handleProximaEvent(const ProximaEvent& event) override;

            Line(const Line&) = delete;
            Line& operator= (const Line&) = delete;
            Line(Line&&) = delete;
            Line& operator=(Line&&) = delete;
    };
} // Namespace UI