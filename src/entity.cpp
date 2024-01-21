#include "entity.h"

Entity::Entity(std::string name, int xPos, int yPos, bool direction) {
    std::string imgDir = "content/" + name + "/";
    this->direction = direction;
    this->tile_l= new Tile(imgDir +  name + "-l.png", xPos, yPos);
    this->tile_r = new Tile(imgDir + name + "-r.png", xPos, yPos);
    this->xVel = 0;
    this->yVel = 0;
    this->health = 100;
}

Entity::~Entity() {
    // actually this is automatic and i'm just dumb
    //delete this->tile_l;
    //delete this->tile_r;
}

void Entity::draw(SDL_Surface *screen) {
    if (this->direction == LEFT) {
        this->tile_l->draw(screen);
    } else {
        this->tile_r->draw(screen);
    }
}

int Entity::getXPos() {
    return this->tile_l->pos.x;
}

int Entity::getYPos() {
    return this->tile_l->pos.y;
}

void Entity::updatePos() {
    this->tile_l->pos.x += this->xVel;
    this->tile_l->pos.y += this->yVel;
    this->tile_r->pos.x += this->xVel;
    this->tile_r->pos.y += this->yVel;
}

bool Entity::updateHealth(int offset) {
    this->health += offset;
    if (this->health > 100) {
        this->health = 100;
    }
    return this->health <= 0;
}

bool Entity::isOnFloor(int floorY) {
    if (this->tile_l->pos.y + 128 >= floorY && this->tile_l->pos.y + 128 - this->yVel <= floorY) {
        return true;
    }
    return false;
}

bool Entity::bouncePlatform(Platform plat) {
    if (this->tile_l->pos.x < plat.xPos || this->tile_l->pos.x > plat.xPos + plat.width) {
        return false;
    }

    if (this->isOnFloor(plat.yPos)) {
        this->tile_l->pos.y = plat.yPos - 128;
        this->tile_r->pos.y = plat.yPos - 128;
        this->yVel = 0.0f;
        return true;
    }
    return false;
}

bool Entity::bouncePlatforms(std::vector<Platform *> plats) {
    bool bounced = false;
    for (int i = 0; i < plats.size(); i++) {
        bounced |= this->bouncePlatform(*plats[i]);
    }
    return bounced;
}

//Player logic

//Zombie logic
void Entity::moveZombie(Entity *player, std::vector<Platform *> plats) {
    if (player->getXPos() < this->getXPos()) {
        this->xVel = -3;
    } else {
        this->xVel = 3;
    }
}