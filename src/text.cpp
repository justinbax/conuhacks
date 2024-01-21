#include "text.h"

Text::Text(std::string text, SDL_Color color, TTF_Font *font) {
    this->surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!this->surface) {
        printf("error 1");
    }
}

void Text::draw(SDL_Surface *screen, int xPos, int yPos) {
    SDL_Rect r;
    r.x = xPos;
    r.y = yPos;
    SDL_BlitSurface(this->surface, NULL, screen, &r);
}