#include "player.h"

Player::Player(std::string path) {
    this->tile = Tile(path);
}

void Player::draw(SDL_Surface *screen) {
    this->tile.draw(screen);
}