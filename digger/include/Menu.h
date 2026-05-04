#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "AppContext.h"

enum class MenuAction {
    NONE,
    START_GAME,
    EXIT
};

class Menu {
public:
    explicit Menu(AppContext& context);
    MenuAction update(const SDL_Event& e, const Uint8* key_state);
    void draw();
    void renderText(const std::string& text, int x, int y, bool selected);
    void show_popup(std::string text);
private:
    AppContext& appContext;
    std::vector<std::string> options = {"Play", "Exit"};
    int selected_index = 0;

};

#endif // MENU_H
