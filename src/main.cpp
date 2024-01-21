#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main(int argc, char **argv) {
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

    window = SDL_CreateWindow("Zombie Killer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << "\n";
        return 1;
    }

    screenSurface = SDL_GetWindowSurface(window);

    Entity player("bob", 100, 0, LEFT);
    Entity ground("dirt", 100, 0, LEFT);


    SDL_Event e;
    bool quit = false;

    while (!quit) {
        uint64_t start = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const uint8_t *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W]) {
            if (player.isOnFloor()) {
                player.yVel = -10;
            }
        }

        player.yVel++;
        if (player.yVel > 0) {
            player.yVel = 0;
        }

        if (state[SDL_SCANCODE_A]) {
            player.xVel = -3;
        } else if (state[SDL_SCANCODE_D]) {
            player.xVel = 3;
        } else {
            player.xVel = 0;
        }

        player.updatePos();

        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
        ground.draw(screenSurface);
        player.draw(screenSurface);
        SDL_UpdateWindowSurface(window);

        uint64_t end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}