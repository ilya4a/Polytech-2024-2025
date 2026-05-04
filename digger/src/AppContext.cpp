#include "AppContext.h"

#include <iostream>
#include <SDL_image.h>

#include "Constants.h"

using namespace std;

AppContext::AppContext() {
    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;

    bool ok = true;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Can't init SDL: " << SDL_GetError() << endl;
        ok = false;
    }
    win = SDL_CreateWindow("lab2", SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SCREEN_WIDTH, SCREEN_HEIGHT,
                                     SDL_WINDOW_SHOWN);
    if (win == NULL) {
        cout << "Can't create window: " << SDL_GetError() << endl;
        ok = false;
    }

    ren = SDL_CreateRenderer(win, -1,
                                       SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        cout << "Can't create renderer: " << SDL_GetError() << endl;
        ok = false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
        ok = false;
    }

    font = TTF_OpenFont("assets/fonts/DejaVuSans.ttf", 20);
    if (!font) {
        std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
        ok = false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Failed to init SDL_image: %s", IMG_GetError());
    }

    context_errors = ok;
}

AppContext::~AppContext() {

    SDL_DestroyWindow(win);
    win = NULL;

    SDL_DestroyRenderer(ren);
    ren = NULL;

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
}


