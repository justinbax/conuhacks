#include "entity.h"

Entity::Entity(std::string name, int xPos, int yPos, bool direction) {
    std::string imgDir = "content/" + name + "/";
    this->direction = direction;
    this->tile_l= new Tile(imgDir +  name + "-l.png", xPos, yPos);
    this->tile_r = new Tile(imgDir + name + "-r.png", xPos, yPos);
    this->xVel = 0;
    this->yVel = 0;
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

void Entity::updatePos() {
    this->tile_l->move(this->xVel, this->yVel);
    this->tile_l->move(this->xVel, this->yVel);
}

void Entity::updateHealth(int offset) {
    this->health += offset;
    if (this->health > 100) {
        this->health = 100;
    }
}

bool Entity::isOnFloor() {
    return true;
}

//Player logic

//Zombie logic
void Entity::movementZombie() {
    //if player is moving
    //move player
    //if player is not moving
    //stop player
    //repeat
}

void Entity::damageZombie() {
    //if zombie is hit
    //zombie health - 1
    //if zombie health = 0
    //kill zombie
    //repeat
}

