#include "player.h"

Player::Player(std::string path) {
    this->tile = Tile(path);
}

void Player::draw() {
    this->tile.draw();
}