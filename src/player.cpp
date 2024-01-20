#include "player.h"

Player::Player(std::string path) {
    this->tile = new Tile(path);
}

Player::~Player() {
    delete this->tile;
}

void Player::draw(SDL_Surface *screen) {
    this->tile->draw(screen);
}

void Player::updateHealth(int offset) {
    this->health += offset;
    if (this->health > 100) {
        this->health = 100;
    }
}