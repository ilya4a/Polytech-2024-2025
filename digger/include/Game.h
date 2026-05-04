#ifndef GAME_H
#define GAME_H

#include <memory>
#include "AppContext.h"
#include "GameManager.h"
#include "Menu.h"

class Game {
    AppContext appContext;
    std::unique_ptr<GameManager> gameManager;
    Menu menu;
    void show_popup(std::string text);
public:
    void run();
    Game();
};


#endif //GAME_H
