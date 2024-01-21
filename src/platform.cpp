#include "platform.h"

Platform::Platform(std::string path, int width, int xPos, int yPos) {
    this->tile = new Tile(path, xPos, yPos);
    this->width = width;
    this->xPos = xPos;
    this->yPos = yPos;
}

Platform::~Platform() {
    //delete this->tile;
}

void Platform::draw(SDL_Surface *screen) {
    this->tile->draw(screen);
}