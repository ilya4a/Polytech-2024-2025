#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>


class AppContext {

public:
    SDL_Window* win;
    SDL_Renderer* ren;
    TTF_Font* font;
    int width;
    int height;
    bool context_errors = true;

    AppContext();
    ~AppContext();
};



#endif //APPCONTEXT_H
