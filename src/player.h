#ifndef PLAYER_H
#define PLAYER_H

#include "tile.h"

class Player {
    public:
        Player(std::string path);
        void draw(SDL_Surface *screen);

    private:
        int health;
        Tile tile;
};

#endif // ifndef PLAYER_H