#include "tile.h"

#include <iostream>

#include <SDL2/SDL_image.h>

Tile::Tile(std::string path, int xPos, int yPos) {
    if (path.size() == 0) {
        this->surface = NULL;
    } else {
        this->surface = IMG_Load(path.c_str());
        if (this->surface == NULL) {
            std::cout << "Couldn't load BMP " << path << "\n";
        }
    }

    this->pos = SDL_Rect();
    this->pos.x = xPos;
    this->pos.y = yPos;
    this->pos.w = 0;
    this->pos.h = 0;
}

Tile::~Tile() {
    if (this->surface) {
        SDL_FreeSurface(this->surface);
        this->surface = NULL;
    }
}

void Tile::draw(SDL_Surface *screen) {
    SDL_BlitSurface(this->surface, NULL, screen, &this->pos);
}

void Tile::move(int xOffset, int yOffset) {
    this->pos.x += xOffset;
    this->pos.y += yOffset;
}