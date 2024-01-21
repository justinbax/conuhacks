#include "entity.h"

Entity *getBestLadder(Entity *zombie, std::vector<Entity *> ladders, bool goUp) {
    int desiredLadderY = zombie->getYPos() + (goUp ? -128 : 128);
    Entity *bestLadder = NULL;
    for (int i = 0; i < ladders.size(); i++) {
        if (abs(ladders[i]->getYPos() - desiredLadderY) < 16) {
            // Possible ladder
            if (!bestLadder) {
                bestLadder = ladders[i];
            } else {
                if (abs(bestLadder->getXPos() - zombie->getXPos()) > abs(ladders[i]->getXPos() - zombie->getXPos())) {
                    bestLadder = ladders[i];
                }
            }
        }
    }
    return bestLadder;
}

Entity::Entity(std::string name, int xPos, int yPos, bool direction) {
    std::string imgDir = "content/" + name + "/";
    this->direction = direction;
    this->tile_l= new Tile(imgDir +  name + "-l.png", xPos, yPos);
    this->tile_r = new Tile(imgDir + name + "-r.png", xPos, yPos);
    this->xVel = 0;
    this->yVel = 0;
    this->health = 100;
    this->cancelGravity = false;
    this->zombieTargetLadder = NULL;
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
void Entity::moveZombie(Entity *player, std::vector<Platform *> plats, std::vector<Entity *> ladders) {
    if (this->cancelGravity) {
        // Was already climbing a ladder; continue climbing
        if (this->yVel > 0) {
            if (this->getYPos() + 131 >= this->zombieTargetLadder->getYPos() + 150) {
                // Stop climbing
                this->cancelGravity = false;
                this->zombieTargetLadder = NULL;
                this->yVel = 0;
            }
        } else {
            // Was climbing up
            if (this->getYPos() + 125 <= this->zombieTargetLadder->getYPos()) {
                // Stop climbing
                this->cancelGravity = false;
                this->zombieTargetLadder = NULL;
                this->yVel = 0;
            }
        }
        return;
    }

    if (this->getYPos() - player->getYPos() >= 8) {
        // Find ladder to go up
        Entity *bestLadder = getBestLadder(this, ladders, true);
        if (bestLadder) {
            // If in middle, start climbing
            if (abs(bestLadder->getXPos() - this->getXPos()) < 8) {
                this->xVel = 0;
                this->cancelGravity = true;
                this->zombieTargetLadder = bestLadder;
                this->yVel = -3;
                return;
            } else {
                this->direction = (bestLadder->getXPos() > this->getXPos() ? RIGHT : LEFT);
                this->xVel = (this->direction == RIGHT ? 3 : -3);
                return;
            }
        }

    } else if (this->getYPos() - player->getYPos() <= -8) {
        // Find ladder to go down
        Entity *bestLadder = getBestLadder(this, ladders, false);
        if (bestLadder) {
            // If in middle, start climbing
            if (abs(bestLadder->getXPos() - this->getXPos()) < 8) {
                this->xVel = 0;
                this->cancelGravity = true;
                this->zombieTargetLadder = bestLadder;
                this->yVel = 3;
                this->tile_l->pos.y += 4;
                this->tile_r->pos.y += 4;
                return;
            } else {
                this->direction = (bestLadder->getXPos() > this->getXPos() ? RIGHT : LEFT);
                this->xVel = (this->direction == RIGHT ? 3 : -3);
                return;
            }
        }
    }
    // Sideways stuff
    if (player->getXPos() < this->getXPos()) {
        this->xVel = -3;
    } else {
        this->xVel = 3;
    }
    // TODO finish completely climbing the ladder
}