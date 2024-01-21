#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "entity.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

Entity *spawnZombie() {
    // 50% normal - 25% small & fast - 25% big & slow
    int type = rand() % 4;
    int spot = rand() % 5;
    int posX;

    posX = spot == 0 ? 68 : spot == 1 ? 218 : spot == 2 ? 368 : spot == 3 ? 518 : 668;

    if (type == 0 || type == 1) {
        Entity *tmp = new Entity("zombie_normal", posX, 0, LEFT);
        return tmp;
    } else if (type == 2) {
        Entity *tmp = new Entity("zombie_fast", posX, 0, LEFT);
        return tmp;
    } else if (type == 3) {
        Entity *tmp = new Entity("zombie_big", posX, 0, LEFT);
        return tmp;
    }

    return NULL;
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
    std::cout << "A shooter game\n";

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    int imgFlags = IMG_INIT_PNG;
    int zombieKilled = 0;

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

    // Background elements
    Entity backdrop("backdrop", 0, 0, LEFT);
    Entity buildings_silhouette("buildings_silhouette", 0, 0, LEFT);
    Entity far_buildings("far_buildings", 0, 0, LEFT);
    Entity buildings_fore("buildings_fore", 0, 0, LEFT);

    
    // Active elements   
    Entity player("shooter", 20, 450, LEFT);
    Entity ground("dirt", 100, 0, LEFT);

    // Other elements
    std::vector<Entity *> bullets;
    std::vector<Entity *> zombies;
    std::vector<Entity *> hearts;
    uint32_t lastShot = SDL_GetTicks();
    uint32_t lastHit = SDL_GetTicks();

    std::vector<Platform *> platforms;
    std::vector<Entity *> ladders;
    
    platforms.push_back(new Platform("content/transparent.png", 1000, -100, 600));
    platforms.push_back(new Platform("content/500/500-r.png", 500, 150, 125));
    platforms.push_back(new Platform("content/300/300-r.png", 300, 0, 275));
    platforms.push_back(new Platform("content/300/300-r.png", 300, 500, 275));
    platforms.push_back(new Platform("content/500/500-r.png", 500, 150, 425));

    ladders.push_back(new Entity("ladder", 150, 125, LEFT));
    ladders.push_back(new Entity("ladder", 650-64, 125, LEFT));
    ladders.push_back(new Entity("ladder", 300-64, 275, LEFT));
    ladders.push_back(new Entity("ladder", 500, 275, LEFT));
    ladders.push_back(new Entity("ladder", 150, 425, LEFT));
    ladders.push_back(new Entity("ladder", 650-64, 425, LEFT));

    hearts.push_back(new Entity("heart", 10, 5, LEFT));
    hearts.push_back(new Entity("heart", 52, 5, LEFT));
    hearts.push_back(new Entity("heart", 94, 5, LEFT));
    hearts.push_back(new Entity("heart", 136, 5, LEFT));
    hearts.push_back(new Entity("heart", 178, 5, LEFT));

    SDL_Event e;
    bool quit = false;
    int zombieCounter = 0;
    int zombieSpawned = 0;
    int zombieLevel = 0;
    int zombieDelay = 360;

    while (!quit) {
        uint64_t start = SDL_GetPerformanceCounter();

        // Zombie generator - Each second
        if (zombieCounter % 360 == 0) {
            zombies.push_back(spawnZombie());
            zombieCounter = 0;
            zombieSpawned++;
        } zombieCounter++;

        // Zombie level up - Each 10 zombies
        if (zombieSpawned % 10 == 0 && zombieSpawned != 0) {
            if (zombieLevel < 4) {
                zombieLevel++;
                zombieDelay -= 60;
            }
        }

        // Poll for events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                continue;
            }
        }

        // Handle keyboard input
        const uint8_t *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W]) {
            if (player.bouncePlatform(*platforms[0])) {
                player.yVel = -7;
            }
        }

        // Physics logic
        player.yVel = (player.yVel >= 5.0f ? player.yVel : player.yVel + 0.3f);
        // Moving left and right
        if (state[SDL_SCANCODE_A]) {
            player.xVel = -4;
            player.direction = LEFT;
        } else if (state[SDL_SCANCODE_D]) {
            player.xVel = 4;
            player.direction = RIGHT;
        } else {
            player.xVel = 0;
        }

        if (state[SDL_SCANCODE_SPACE] && SDL_GetTicks() > lastShot + 500) {
            int bulletX = player.getXPos() + (player.direction == LEFT ? 0 : 48);
            shoot(bullets, bulletX, player.getYPos() + 50, player.direction);
            lastShot = SDL_GetTicks();
        }

        player.updatePos();
        player.bouncePlatforms(platforms);

        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
        // Background drawing
        backdrop.draw(screenSurface);
        buildings_silhouette.draw(screenSurface);
        far_buildings.draw(screenSurface);
        buildings_fore.draw(screenSurface);

        for (int i = 0; i < hearts.size(); i++) {
            hearts[i]->draw(screenSurface);
        }

        for (int i = 0; i < platforms.size(); i++) {
            platforms[i]->draw(screenSurface);
        }
        for (int i = 0; i < ladders.size(); i++) {
            ladders[i]->draw(screenSurface);
        }

        // Active elements drawing
        player.draw(screenSurface);
        
        for (int i = 0; i < zombies.size(); i++) {
            zombies[i]->draw(screenSurface);
            zombies[i]->moveZombie(&player, platforms, ladders);
            zombies[i]->updatePos();
            // Gravity
            zombies[i]->yVel += 0.3f;
            zombies[i]->bouncePlatforms(platforms);
        }

        for (int i = 0; i < bullets.size(); i++) {
            bullets[i]->draw(screenSurface);
            bullets[i]->updatePos();

            // TODO check if out of bounds
            for (int j = 0; j < zombies.size(); j++) {
                if (bullets[i]->getXPos() - zombies[j]->getXPos() > 0 && bullets[i]->getXPos() - zombies[j]->getXPos() < 64
                && bullets[i]->getYPos() - zombies[j]->getYPos() > 0 && bullets[i]->getYPos() - zombies[j]->getYPos() < 128) {
                    bullets.erase(bullets.begin() + i);
                    
                    // Collision
                    if (zombies[j]->updateHealth(-25)) {
                        zombies.erase(zombies.begin() + j);
                        zombieKilled++;
                        std::cout << "You killed " << zombieKilled << " zombies!\n";
                        std::cout << "Your score is " << zombieKilled * 100 << "!\n";
                        j--;
                    }
                }
            }
        }

        // Player x Zombie collision handling
        for (int i = 0; i < zombies.size(); i++) {
            if (abs(zombies[i]->getXPos() - player.getXPos()) <= 65 && abs(zombies[i]->getYPos() - player.getYPos()) <= 129 && SDL_GetTicks() > lastHit + 1000) {
                player.updateHealth(-20);
                hearts.erase(hearts.begin() + hearts.size() - 1);
                if (player.updateHealth(0)) {
                    quit = true;
                }
                lastHit = SDL_GetTicks();
            }
        }

        SDL_UpdateWindowSurface(window);

        uint64_t end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));
    }

    std::cout << "You killed " << zombieKilled << " zombies!\n";
    std::cout << "Your score is " << zombieKilled * 100 << "!\n";

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}