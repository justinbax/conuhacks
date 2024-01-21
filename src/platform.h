#ifndef PLATFORM_H
#define PLATFORM_H

#include "tile.h"

class Platform {
    public:
        Platform(std::string path, int width, int xPos, int yPos);
        ~Platform();
        void draw(SDL_Surface *screen);
        int width;
        int xPos;
        int yPos;
    private:
        Tile *tile;
};

#endif // ifndef PLATFORM_H`