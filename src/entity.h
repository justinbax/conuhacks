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
        ~Entity();
        void draw(SDL_Surface *screen);
        int getXPos();
        int getYPos();
        void updatePos();
        bool updateHealth(int offset);
        bool isOnFloor(int floorY);
        bool bouncePlatform(Platform plat);
        bool bouncePlatforms(std::vector<Platform *> plats);
        void moveZombie(Entity *player, std::vector<Platform *> plats);
        float xVel;
        float yVel;
        bool direction;

    private:
        int health;
        Tile *tile_l;
        Tile *tile_r;
};

#endif // ifndef Entity_H