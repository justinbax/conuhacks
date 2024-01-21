#include <iostream>
#include <fstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>

#include "entity.h"
#include "text.h"

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
    int score = 0;

    // Initialize SDL - Throw error message if needed
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << "\n";
        return 1;
    }

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
        return 1;
    }

    if ((TTF_Init())) {
        std::cout << "SDL_ttf couldn't initialize: SDL_ttf Error: " << TTF_GetError() << "\n";
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
    std::vector<std::string> shootersTiles = {"shooter1", "shooter2", "shooter3", "shooter4", "shooter5", "shooter6"};
    Entity player(shootersTiles, 20, 450, LEFT);
    Entity ground("dirt", 100, 0, LEFT);

    // Other elements
    std::vector<Entity *> bullets;
    std::vector<Entity *> zombies;
    std::vector<Entity *> hearts;
    uint32_t lastShot = SDL_GetTicks();
    uint32_t lastHit = SDL_GetTicks();
    uint32_t lastAnimationTick = SDL_GetTicks();

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

    // Text stuff
    TTF_Font *font = TTF_OpenFont("content/OpenSans.ttf", 24);
    if (!font) std::cout << "no font";
    SDL_Color white = {0xFF, 0xFF, 0xFF};

    bool gameOver = false;
    bool welcomeScreen = true;
    SDL_Event e;
    bool quit = false;
    int zombieCounter = 0;
    int zombieSpawned = 0;
    int zombieLevel = 0;
    int zombieDelay = 360;

    // Read high score
    int highscore = 0;
    std::ifstream highScoreFile("highscore.txt");
    std::string highScoreString;
    std::getline(highScoreFile, highScoreString);
    int highScore = strtol(highScoreString.c_str(), NULL, 10);
    highScoreFile.close();

    while (!quit) {
        uint64_t start = SDL_GetPerformanceCounter();

        // Poll for events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                continue;
            }
        }

        if (gameOver) {
            std::string gameOverText = "Game over! You scored ";
            gameOverText.append(std::to_string(score));
            gameOverText.append(" points.");
            std::string gameOverText2 = "High score: ";
            gameOverText2.append(std::to_string(highScore));
            Text gameOver(gameOverText, white, font);
            Text gameOver2(gameOverText2, white, font);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
            backdrop.draw(screenSurface);
            buildings_silhouette.draw(screenSurface);
            far_buildings.draw(screenSurface);
            buildings_fore.draw(screenSurface);
            gameOver.draw(screenSurface, 200, 300);
            gameOver2.draw(screenSurface, 300, 350);
            SDL_UpdateWindowSurface(window);

            uint64_t end = SDL_GetPerformanceCounter();
            float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
            SDL_Delay(floor(16.666f - elapsedMS));
            continue;

        } else if (welcomeScreen) {
            std::string welcomeText = "Welcome to Deadly Byte.\nPress space to start...";
            Text welcome(welcomeText, white, font);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));
            backdrop.draw(screenSurface);
            buildings_silhouette.draw(screenSurface);
            welcome.draw(screenSurface, 150, 300);
            SDL_UpdateWindowSurface(window);

            const uint8_t *state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_SPACE]) {
                welcomeScreen = false;
            }

            uint64_t end = SDL_GetPerformanceCounter();
            float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
            SDL_Delay(floor(16.666f - elapsedMS));
            continue;
        }

        // Handle keyboard input
        const uint8_t *state = SDL_GetKeyboardState(NULL);
        // If already on ladder
        if (player.cancelGravity) {
            // Check if still close enough to ladder
            if (player.targetLadder->getXPos() - player.getXPos() > 32 || player.targetLadder->getXPos() - player.getXPos() < -32
            || player.getYPos() + 125 >= player.targetLadder->getYPos() || player.getYPos() + 128 <= player.targetLadder->getYPos() + 200) {
                // Cancel the being on ladder
                player.cancelGravity = false;
                player.yVel = 0;
                player.targetLadder = NULL;
            }

            // Check if W is pressed or not
            if (state[SDL_SCANCODE_W]) {
                player.yVel = 3;
            } else if (state[SDL_SCANCODE_S]) {
                player.yVel = -3;
            } else {
                player.yVel = 0;
            }
        }
        if (state[SDL_SCANCODE_W]) {
            // Search if player is on ladder
            for (int i = 0; i < ladders.size(); i++) {
                if (ladders[i]->getXPos() - player.getXPos() < 32 && ladders[i]->getXPos() - player.getXPos() > -32) {
                    if (player.getYPos() + 128 >= ladders[i]->getYPos() && player.getYPos() + 128 <= ladders[i]->getYPos() + 200) {
                        player.targetLadder = ladders[i];
                        player.cancelGravity = true;
                        player.yVel = -3;
                        break;
                    }
                }
            }

            // Else jumps
            if (player.bouncePlatform(*platforms[0])) {
                player.yVel = -7;
                player.switchFrame();
            }
        }

        // Physics logic
        player.yVel = (player.yVel >= 5.0f || player.cancelGravity ? player.yVel : player.yVel + 0.3f);
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
            int bulletX = player.getXPos() + (player.direction == LEFT ? 0 : 64);
            shoot(bullets, bulletX, player.getYPos() + 60, player.direction);
            lastShot = SDL_GetTicks();
        }

        player.updatePos();
        player.bouncePlatforms(platforms);

        // Update player's animation
        if (SDL_GetTicks() > lastAnimationTick + 200 && player.xVel != 0) {
            player.switchFrame();
            lastAnimationTick = SDL_GetTicks();
        }

        // Spawn zombies and stuff
        // Zombie generator - Each second
        if (zombieCounter % zombieDelay == 0) {
            zombies.push_back(spawnZombie());
            zombieCounter = 0;
            zombieSpawned++;
        } zombieCounter++;

        // Zombie level up - Each 10 zombies
        if (zombieSpawned % 10 == 0 && zombieSpawned != 0) {
            if (zombieLevel < 5) {
                zombieLevel++;
                zombieDelay -= 40;
                std::cout << "Zombie level up! Level " << zombieLevel << "\n";
            }
        }

        // Draw stuff
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
                    score += 10;

                    // Collision
                    if (zombies[j]->updateHealth(-25)) {
                        zombies.erase(zombies.begin() + j);
                        score += 100;
                        j--;
                    }
                }
            }
        }

        // Player x Zombie collision handling
        for (int i = 0; i < zombies.size(); i++) {
            if (abs(zombies[i]->getXPos() - player.getXPos()) <= 65 && abs(zombies[i]->getYPos() - player.getYPos()) <= 129 && SDL_GetTicks() > lastHit + 1500) {
                player.updateHealth(-20);
                hearts.erase(hearts.begin() + hearts.size() - 1);
                if (player.updateHealth(0)) {
                    gameOver = true;
                }
                lastHit = SDL_GetTicks();
            }
        }

        SDL_UpdateWindowSurface(window);

        uint64_t end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));
    }

    std::cout << "Your score is " << score << "!\n";
    std::cout << "100 points per kill, 10 points per hit.\n";

    if (score > highScore) {
        std::ofstream highScoreFile("highscore.txt", std::ofstream::trunc);
        highScoreFile << score;
        highScoreFile.close();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}