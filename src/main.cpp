#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"

#define SCR_WIDTH 640
#define SCR_HEIGHT 480

int main() {
    std::cout << "Hello, world!";

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
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));

    Player player("bob.png");
    
    player.draw(screenSurface);
    SDL_UpdateWindowSurface(window);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}