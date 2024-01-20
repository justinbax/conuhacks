#include "tile.h"

Tile::Tile(std::string path) {
    if (path.size() == 0) {
        this->surface = NULL;
    } else {
        this->surface = SDL_LoadBMP(path.c_str());
    }
}

Tile::~Tile() {
    if (this->surface) {
        SDL_FreeSurface(this->surface);
        this->surface = NULL;
    }
}

void Tile::draw(SDL_Surface *screen) {
    SDL_BlitSurface(this->surface, NULL, screen, NULL);
}