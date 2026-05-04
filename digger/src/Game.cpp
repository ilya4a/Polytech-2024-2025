#include "Game.h"
#include <memory>
#include "Constants.h"
#include "Menu.h"
#include "Alien.h"
#include <SDL.h>
#include <string>
#include <iostream>

using namespace std;

Game::Game() : appContext(), menu(appContext){
    gameManager = make_unique<GameManager>(appContext);
}

void Game::show_popup(std::string text) {
    gameManager->draw(appContext);
    menu.show_popup(text);

    bool waiting = true;
    SDL_Event e;
    while (waiting) {
        while (SDL_PollEvent(&e)) {
            if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) ||
                e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                waiting = false;
                break;
            }
        }
        SDL_Delay(DELAY);
    }
}


void Game::run() {
    bool in_menu = true;
    bool run = true;
    bool is_win = false;

    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time;
    double delta_time;
    SDL_Event e;

    while (run) {
        const Uint8* key_state = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                run = false;
            }
            if (in_menu) {
                MenuAction action = menu.update(e, key_state);
                if (action == MenuAction::START_GAME) {
                    Alien::reset_order();
                    gameManager = std::make_unique<GameManager>(appContext);
                    last_time = SDL_GetTicks();
                    in_menu = false;
                } else if (action == MenuAction::EXIT) {
                    run = false;
                }
            }
        }

        if (in_menu) {
            menu.draw();
            SDL_Delay(16);
            continue;
        }

        if(gameManager->is_win()){
            is_win = true;
        }
        current_time = SDL_GetTicks();
        delta_time = (current_time - last_time);
        last_time = current_time;

        bool is_loose = gameManager->update(delta_time);
        gameManager->draw(appContext);

        menu.renderText("score = " + to_string(gameManager->get_score()), 10, 10, false);
        SDL_RenderPresent(appContext.ren);
        if (is_loose) {
            show_popup("Game over :( press enter");
            in_menu = true;
            gameManager.reset();
        }else if(is_win){
            is_win = false;
            show_popup("WIN!!! press enter");
            in_menu = true;
            gameManager.reset();
        }
        SDL_Delay(DELAY);
    }
}





