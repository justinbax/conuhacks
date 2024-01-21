#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "entity.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

Entity spawnZombie() {
    //zombie is the name of the folder! Need to update the pixel art
    Entity tmp("zombie", 64, 64, LEFT);
    return tmp;
}

void shoot(std::vector<Entity *> &bullets, int xPos, int yPos, bool direction) {
    Entity *bullet = new Entity("bullet", xPos, yPos, direction);
    if (direction == LEFT) {
        bullet->xVel = -5;
    } else {
        bullet->xVel = 5;
    }

    bullets.push_back(bullet);
}

int main(int argc, char **argv) {
    std::cout << "A shooter game";

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    int imgFlags = IMG_INIT_PNG;

    // Initialize SDL - Throw error message if needed
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

    // Get window surface
    screenSurface = SDL_GetWindowSurface(window);

    Entity backdrop("backdrop", 100, 0, LEFT);
    Entity buildings_silhouette("buildings_silhouette", 100, 0, LEFT);
    Entity far_buildings("far_buildings", 100, 0, LEFT);
    Entity buildings_fore("buildings_fore", 100, 0, LEFT);
    Entity player("shooter", 20, 536, LEFT);
    Entity zombie = spawnZombie();
    Entity ground("dirt", 100, 0, LEFT); // Temporary coordinates - Will be removed next pull
    std::vector<Entity *> bullets;
    std::vector<Entity *> zombies;
    uint32_t lastShot = SDL_GetTicks();

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        uint64_t start = SDL_GetPerformanceCounter();

        // Poll for events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Handle keyboard input
        const uint8_t *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W]) {
            if (player.isOnFloor()) {
                player.yVel = -7;
            }
        }

        // Physics logic
        player.yVel += 0.3f;
        if (player.yVel >= 0 && player.isOnFloor()) {
            player.yVel = 0;
        }
        // Moving left and right
        if (state[SDL_SCANCODE_A]) {
            player.xVel = -3;
            player.direction = LEFT;
        } else if (state[SDL_SCANCODE_D]) {
            player.xVel = 3;
            player.direction = RIGHT;
        } else {
            player.xVel = 0;
        }

        if (state[SDL_SCANCODE_SPACE] && SDL_GetTicks() > lastShot + 500) {
            shoot(bullets, player.getXPos(), player.getYPos(), player.direction);
            lastShot = SDL_GetTicks();
        }

        player.updatePos();

        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
        backdrop.draw(screenSurface);
        buildings_silhouette.draw(screenSurface);
        far_buildings.draw(screenSurface);
        buildings_fore.draw(screenSurface);

        ground.draw(screenSurface);
        player.draw(screenSurface);
        zombie.draw(screenSurface);
        zombie.updatePos();
        
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i]->draw(screenSurface);
            bullets[i]->updatePos();

            // TODO check if out of bounds
            for (int j = 0; j < zombies.size(); j++) {
                if (bullets[i]->getXPos() - zombies[j]->getXPos() < 32 && bullets[i]->getXPos() - zombies[j]->getXPos() < 64) {
                    // Collision
                    std::cout << "collision";
                    zombies[j]->updateHealth(-10);
                    Entity *bullet = bullets[i];
                    bullets.erase(bullets.begin() + i);
                    delete bullet;
                }
            }
        }

        SDL_UpdateWindowSurface(window);

        uint64_t end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));

        //TODO - checkQuitConditions();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}