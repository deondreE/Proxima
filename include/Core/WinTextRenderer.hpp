#pragma once
#include "Core/TextRenderer.hpp" 
#include <Windows.h>
#include <iostream>
#include <filesystem>

namespace Core {

  class WinFont : public Font {
    public:
      HFONT hFont;
      int size;
      WinFont(HFONT f, int s) : hFont(f), size(s) {}
      ~WinFont() {
        if (hFont) DeleteObject(hFont);
      }
      
      int getSize() const override { return size; }
  };

  class WinTextRenderer : public TextRenderer {
    HDC hdc;
    public:
      explicit WinTextRenderer(HDC context) : hdc(context) {}

      Font* loadFont(const std::string& file, int pSize) {
        if (AddFontResourceExA(file.c_str(), FR_PRIVATE, 0) == 0) {
          return loadSystemFont("Arial", pSize);
        }

        std::string fontName = getFontNameFromFile(file);

          HFONT font = CreateFontA(
          pSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
          ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
          DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,  // Fixed: | not ||
          fontName.c_str()  // Use actual font name
        );
    
        return new WinFont(font, pSize);
      }

      void drawText(const std::string& text, Font* font, const Color& color, int x, int y, int maxWidth, int maxHeight, bool wordWrap) override {
        auto winFont = dynamic_cast<WinFont*>(font);
        if (!winFont) return; 
        HFONT oldFont = (HFONT)SelectObject(hdc, winFont->hFont);
        SetTextColor(hdc, RGB(color.r, color.g, color.b)); 
        SetBkMode(hdc, TRANSPARENT);

        RECT textRect = {x, y, x + maxWidth, y+maxHeight};

        UINT format = DT_LEFT; // left aligned
        if (wordWrap) {
          format |= DT_WORDBREAK;
        }
        if (maxHeight == 0) {
          format |= DT_NOCLIP;
        } 

        DrawTextA(hdc, text.c_str(), (int)text.size(), &textRect, format);
        SelectObject(hdc, oldFont);
      }

      std::pair<int, int> measureText(const std::string& text, Font* font) override {
        SIZE size{0, 0};
        auto winFont = dynamic_cast<WinFont*>(font);
        if (winFont) {
          HFONT old = (HFONT)SelectObject(hdc, winFont->hFont);
          GetTextExtentPoint32A(hdc, text.c_str(), (int)text.size(), &size); 
          SelectObject(hdc, old);
        }
        return {size.cx, size.cy};
      }

      std::pair<int, int> measureWrappedText(const std::string& text, Font* font, int maxWidth) override {
          SIZE size{0, 0};
          auto winFont = dynamic_cast<WinFont*>(font);
          if (winFont && hdc) {
            HFONT old = (HFONT)SelectObject(hdc, winFont->hFont);
            RECT bounds = {0, 0, maxWidth, 0};
            DrawTextA(hdc, text.c_str(), (int)text.size(), &bounds, DT_WORDBREAK | DT_CALCRECT);
            size.cx = bounds.right - bounds.left;
            size.cy = bounds.bottom - bounds.top;
            SelectObject(hdc, old);
          }
          return {size.cx, size.cy};
      }

      Font* loadSystemFont(const std::string& fontName, int pSize) {
          HFONT font = CreateFontA(
              pSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
              ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
              DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
              fontName.c_str()  // "Arial", "Times New Roman", etc.
          );
          return new WinFont(font, pSize);
      }

      std::string getFontNameFromFile(const std::string& file) {
          // Temporarily add font to get its name
          int numFonts = AddFontResourceExA(file.c_str(), FR_PRIVATE, 0);
          if (numFonts == 0) return "Arial";  // Fallback
          
          // Use GDI+ or enumerate fonts to get the actual name
          // This is complex, so simpler approach below...
          
          // For now, extract from filename (simple heuristic)
          size_t lastSlash = file.find_last_of("/\\");
          size_t lastDot = file.find_last_of('.');
          std::string name = file.substr(lastSlash + 1, lastDot - lastSlash - 1);
          return name;
      }
  };
}