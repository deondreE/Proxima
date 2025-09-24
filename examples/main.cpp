#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <memory>

#include "UI/View.hpp"
#include "UI/Button.hpp"
#include "UI/StackLayout.hpp"
#include "UI/ConfigManager.hpp"
#include "UI/Text.hpp"

using namespace UI;

TTF_Font* g_ui_font = nullptr;

void on_btn_clicked() {
    std::cout << "Button clicked!\n";
}

ConfigManager configManager;

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!TTF_Init()) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << std::endl;
        SDL_Quit(); // Ensure SDL is also quit if TTF fails
        return 1;
    }

    if (!configManager.load("./examples/config/app_config.yaml")) {
        std::cerr << "Failed to load config\n";
        SDL_Quit();
        TTF_Quit();
        return 1;
    }
    const AppConfig& appConfig = configManager.getAppConfig();

    SDL_Window* window = SDL_CreateWindow("SDL3 Test Window", appConfig.initial_width, appConfig.initial_height, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    g_ui_font = TTF_OpenFont("./examples/config/fonts/Delius-Regular.ttf", 24);
    if (!g_ui_font) {
        std::cerr << "Failed to load font! TTF_Error: " << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    } else {
        std::cout << "worked!";
    }

    Text hello("Hello Proxima UI");
    hello.size(300, 30);
    hello.pos(0, -100);
    hello.setFont(g_ui_font);
    hello.setColor({0,0,0,255});

    Button btn;
    btn.size(120, 40)
        .pos(10, 60)
        .setFont(g_ui_font)
        .text("Press this button!")
        .onClick(on_btn_clicked);
    btn.setColor({255, 255, 255, 255});

    StackLayout layout;
    layout.orientation(Vertical).spacing(20);
    layout.add(hello);
    layout.pos(100, 100);
    layout.add(btn);
    layout.add(hello);

    View root;
    root.size(appConfig.initial_width, appConfig.initial_height);
    root.pos(0, 0);
    root.add(layout);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) { // ESC key
                    running = false;
                }
            }
        }

        // Clear with blue background
        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
        SDL_RenderClear(renderer);

        // Draw a simple rectangle to test basic rendering
        // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // SDL_FRect rect = {50, 50, 200, 100};
        // SDL_RenderFillRect(renderer, &rect);

        root.layout(0, 0);
        root.draw(renderer);

        // if (g_ui_font) {
        //     std::string s = "Test text";
        //     SDL_Surface* textSurface = TTF_RenderText_Solid(g_ui_font, s.c_str(), s.length(), {0, 0, 0, 255});
        //     if (textSurface) {
        //         SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        //         if (textTexture) {
        //             SDL_FRect textRect = {60, 60, (float)textSurface->w, (float)textSurface->h};
        //             SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
        //             SDL_DestroyTexture(textTexture);
        //         }
        //         SDL_DestroySurface(textSurface);
        //     }
        // }

        SDL_RenderPresent(renderer); // Update the screen
    }

    if (g_ui_font) {
        TTF_CloseFont(g_ui_font);
    }
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}