#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text {
    public:
        Text(std::string text, SDL_Color color, TTF_Font *font);
        void draw(SDL_Surface *screen, int xPos, int yPos);
    private:
        SDL_Surface *surface;
};

#endif // ifndef TEXT_H