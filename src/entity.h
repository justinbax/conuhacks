#ifndef Entity_H
#define Entity_H

#include "tile.h"

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
        void updateHealth(int offset);
        bool isOnFloor();
        float xVel;
        float yVel;
        bool direction;

    private:
        int health;
        Tile *tile_l;
        Tile *tile_r;
};

#endif // ifndef Entity_H