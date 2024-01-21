#ifndef TILE_H
#define TILE_H

#include <string>

#include <SDL2/SDL.h>

class Tile {
    public:
        Tile(std::string path = "", int xPos = 0, int yPos = 0);
        ~Tile();
        void draw(SDL_Surface *screen);
        int getXPos();
        int getYPos();
        SDL_Rect pos;

    private:
        SDL_Surface *surface;
};

#endif // ifndef TILE_H