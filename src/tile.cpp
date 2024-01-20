#include "tile.h"

#include <iostream>

Tile::Tile(std::string path) {
    if (path.size() == 0) {
        this->surface = NULL;
    } else {
        this->surface = SDL_LoadBMP(path.c_str());
        if (this->surface == NULL) {
            std::cout << "Couldn't load BMP " << path << "\n";
        }
    }
    std::cout << "Created Tile\n";
}

Tile::~Tile() {
    if (this->surface) {
        SDL_FreeSurface(this->surface);
        this->surface = NULL;
    }
    std::cout << "Deleted Tile\n";
}

void Tile::draw(SDL_Surface *screen) {
    std::cout << this->surface->flags;
    SDL_BlitSurface(this->surface, NULL, screen, NULL);
}