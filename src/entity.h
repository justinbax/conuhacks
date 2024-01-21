#ifndef Entity_H
#define Entity_H

#include <vector>

#include "tile.h"
#include "platform.h"

#define LEFT true
#define RIGHT false
class Entity {
    public:
        Entity(std::string name = "", int xPos = 0, int yPos = 0, bool direction = RIGHT);
        Entity(std::vector<std::string> names, int xPos = 0, int yPos = 0, bool direction = RIGHT);
        ~Entity();
        void draw(SDL_Surface *screen);
        int getXPos();
        int getYPos();
        void updatePos();
        bool updateHealth(int offset);
        bool isOnFloor(int floorY);
        bool bouncePlatform(Platform plat);
        bool bouncePlatforms(std::vector<Platform *> plats);
        void moveZombie(Entity *player, std::vector<Platform *> plats, std::vector<Entity *> ladders);
        void switchFrame();
        float xVel;
        float yVel;
        bool direction;
        bool cancelGravity;
        Entity *targetLadder;

    private:
        int health;
        int currentTile;
        std::vector<Tile *> tiles_l;
        std::vector<Tile *> tiles_r;
};

#endif // ifndef Entity_H