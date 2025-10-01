#include "UI/Renderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h> 
#include <stdexcept>
#include <string>

using namespace UI;

namespace Core {
class SDLRenderer : public Renderer {
 private:
  SDL_Renderer* _renderer;
  SDL_Window* _window;

 public:
  SDLRenderer(SDL_Renderer* renderer, SDL_Window* window) 
	  : _renderer(renderer), _window(window) {
      if (!_renderer || !_window) {
        throw std::runtime_error("SDLRenderer: Provided input, broken");
      }
    }

  virtual ~SDLRenderer() override = default;

  void setDrawColor(const Color& color) override {
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
  }

    void drawRect(int x, int y, int w, int h) override {
    SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
    SDL_RenderRect(_renderer, &rect);
  }

  void fillRect(int x, int y, int w, int h) override {
    SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
    SDL_RenderFillRect(_renderer, &rect);
  }

  void drawLine(int x1, int y1, int x2, int y2) override {
    SDL_RenderLine(_renderer, (float)x1, (float)y1, (float)x2, (float)y2);
  }

  void present() override {
    SDL_RenderPresent(_renderer);
  }

  void clear() override {
    SDL_RenderClear(_renderer);
  }

  SDL_Renderer* getSDLRenderer() const { return _renderer; }
};
} // Namespace Core