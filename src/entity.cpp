#include "entity.h"


Entity::Entity(std::string name, int xPos, int yPos, bool direction) {
    std::string imgDir = "content/" + name + "/";
    this->direction = direction;
    this->tile_l= new Tile(imgDir +  name + "-l.png", xPos, yPos);
    this->tile_r = new Tile(imgDir + name + "-r.png", xPos, yPos);
}

Entity::~Entity() {
    delete this->tile_l;
    delete this->tile_r;
}

void Entity::draw(SDL_Surface *screen) {
    if (this->direction == LEFT) {
        this->tile_l->draw(screen);
    } else {
        this->tile_r->draw(screen);
    }
}

void Entity::move(int xOffset, int yOffset) {
    this->tile_l->move(xOffset, yOffset);
    this->tile_r->move(xOffset, yOffset);
}

void Entity::updateHealth(int offset) {
    this->health += offset;
    if (this->health > 100) {
        this->health = 100;
    }
}