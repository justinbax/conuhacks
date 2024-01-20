#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.h"

#define SCR_WIDTH 640
#define SCR_HEIGHT 480

int main() {
    std::cout << "A shooter game";

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    int imgFlags = IMG_INIT_PNG;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << "\n";
        return 1;
    }

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << "\n";
        return 1;
    }

    screenSurface = SDL_GetWindowSurface(window);

    Entity player("bob", 100, 0, LEFT);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            int numPresses = 0;
            const uint8_t *state = SDL_GetKeyboardState(&numPresses);
            if (state[SDL_SCANCODE_W]) {
                player.move(0, -1);
            } else if (state[SDL_SCANCODE_S]) {
                player.move(0, 1);
            } else if (state[SDL_SCANCODE_A]) {
                player.move(-1, 0);
            } else if (state[SDL_SCANCODE_D]) {
                player.move(1, 0);
            }
        }

        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
        player.draw(screenSurface);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}