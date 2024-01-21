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
    this->tiles_l = {};
    this->tiles_l.push_back(new Tile(imgDir +  name + "-l.png", xPos, yPos));
    this->tiles_r = {};
    this->tiles_r.push_back(new Tile(imgDir +  name + "-r.png", xPos, yPos));
    this->xVel = 0;
    this->yVel = 0;
    this->health = 100;
    this->cancelGravity = false;
    this->targetLadder = NULL;
    this->currentTile = 0;
}

Entity::Entity(std::vector<std::string> names, int xPos, int yPos, bool direction) {
    this->tiles_l = {};
    this->tiles_r = {};
    for (int i = 0; i < names.size(); i++) {
        std::string imgDir = "content/" + names[i] + "/";
        this->tiles_l.push_back(new Tile(imgDir + names[i] + "-l.png", xPos, yPos));
        this->tiles_r.push_back(new Tile(imgDir + names[i] + "-r.png", xPos, yPos));
    }
    this->direction = direction;
    this->xVel = 0;
    this->yVel = 0;
    this->health = 100;
    this->cancelGravity = false;
    this->targetLadder = NULL;
}

Entity::~Entity() {
    // actually this is automatic and i'm just dumb
    //delete this->tile_l;
    //delete this->tile_r;
}

void Entity::draw(SDL_Surface *screen) {
    if (this->direction == LEFT) {
        this->tiles_l[this->currentTile]->draw(screen);
    } else {
        this->tiles_r[this->currentTile]->draw(screen);
    }
}

int Entity::getXPos() {
    return this->tiles_l[this->currentTile]->pos.x;
}

int Entity::getYPos() {
    return this->tiles_l[this->currentTile]->pos.y;
}

void Entity::updatePos() {
    for (int i = 0; i < this->tiles_l.size(); i++) {
        this->tiles_l[i]->pos.x += this->xVel;
        this->tiles_l[i]->pos.y += this->yVel;
        this->tiles_r[i]->pos.x += this->xVel;
        this->tiles_r[i]->pos.y += this->yVel;
    }
}

bool Entity::updateHealth(int offset) {
    this->health += offset;
    if (this->health > 100) {
        this->health = 100;
    }
    return this->health <= 0;
}

bool Entity::isOnFloor(int floorY) {
    if (this->tiles_l[this->currentTile]->pos.y + 128 >= floorY && this->tiles_l[this->currentTile]->pos.y + 128 - abs(this->yVel) <= floorY) {
        return true;
    }
    return false;
}

bool Entity::bouncePlatform(Platform plat) {
    if (this->tiles_l[this->currentTile]->pos.x < plat.xPos || this->tiles_l[this->currentTile]->pos.x > plat.xPos + plat.width) {
        return false;
    }

    if (this->isOnFloor(plat.yPos)) {
        for (int i = 0; i < this->tiles_l.size(); i++) {
            this->tiles_l[i]->pos.y = plat.yPos - 128;
            this->tiles_r[i]->pos.y = plat.yPos - 128;
        }
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

void Entity::switchFrame() {
    this->currentTile++;
    if (this->currentTile >= this->tiles_l.size()) {
        this->currentTile = 0;
    }
}

//Player logic

//Zombie logic
void Entity::moveZombie(Entity *player, std::vector<Platform *> plats, std::vector<Entity *> ladders) {
    if (this->cancelGravity) {
        // Was already climbing a ladder; continue climbing
        if (this->yVel > 0) {
            if (this->getYPos() + 131 >= this->targetLadder->getYPos() + 150) {
                // Stop climbing
                this->cancelGravity = false;
                this->targetLadder = NULL;
                this->yVel = 0;
            }
        } else {
            // Was climbing up
            if (this->getYPos() + 125 <= this->targetLadder->getYPos()) {
                // Stop climbing
                this->cancelGravity = false;
                this->targetLadder = NULL;
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
                this->targetLadder = bestLadder;
                this->yVel = -2.5;
                return;
            } else {
                this->direction = (bestLadder->getXPos() > this->getXPos() ? RIGHT : LEFT);
                this->xVel = (this->direction == RIGHT ? 2.5 : -2.5);
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
                this->targetLadder = bestLadder;
                this->yVel = 2;
                for (int i = 0; i < this->tiles_l.size(); i++) {
                    this->tiles_l[i]->pos.y += 4;
                    this->tiles_r[i]->pos.y += 4;
                }
                return;
            } else {
                this->direction = (bestLadder->getXPos() > this->getXPos() ? RIGHT : LEFT);
                this->xVel = (this->direction == RIGHT ? 2.5 : -2.5);
                return;
            }
        }
    }
    // Sideways stuff
    if (player->getXPos() < this->getXPos()) {
        this->xVel = -2.5;
    } else {
        this->xVel = 2.5;
    }
    // TODO finish completely climbing the ladder
}