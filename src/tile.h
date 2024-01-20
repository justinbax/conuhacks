#ifndef TILE_H
#define TILE_H

#include <string>

#include <SDL2/SDL.h>

class Tile {
    public:
        Tile(std::string path = "", int xPos = 0, int yPos = 0);
        ~Tile();
        void draw(SDL_Surface *screen);
        void move(int xOffset, int yOffset);

    private:
        SDL_Surface *surface;
        SDL_Rect pos;
};

#endif // ifndef TILE_H