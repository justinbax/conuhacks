#ifndef TILE_H
#define TILE_H

#include <string>

#include <SDL2/SDL.h>

class Tile {
    public:
        Tile(std::string path = "");
        ~Tile();
        void draw(SDL_Surface *screen);

    private:
        SDL_Surface *surface;
};

#endif // ifndef TILE_H