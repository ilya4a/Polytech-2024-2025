#include "Menu.h"

Menu::Menu(AppContext& context) : appContext(context) {}

MenuAction Menu::update(const SDL_Event& e, const Uint8* key_state) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                selected_index = (selected_index - 1 + options.size()) % options.size();
                break;
            case SDLK_DOWN:
                selected_index = (selected_index + 1) % options.size();
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if (selected_index == 0) return MenuAction::START_GAME;
                if (selected_index == 1) return MenuAction::EXIT;
                break;
        }
    }
    return MenuAction::NONE;
}

void Menu::draw() {
    SDL_SetRenderDrawColor(appContext.ren, 0, 0, 0, 255);
    SDL_RenderClear(appContext.ren);

    int btn_height = 0;
    int btn_width = 0;
    TTF_SizeText(appContext.font, "Play", &btn_width, &btn_height);
    int spacing = static_cast<int>(btn_height * 0.3);

    int total_height = (btn_height + spacing) * options.size() - spacing;
    int start_y = (appContext.height - total_height) / 2;

    for (int i = 0; i < options.size(); ++i) {
        int text_width = 0, text_height = 0;
        TTF_SizeText(appContext.font, options[i].c_str(), &text_width, &text_height);

        int x = (appContext.width - text_width) / 2;
        int y = start_y + i * (btn_height + spacing);

        renderText(options[i], x, y, i == selected_index);
    }

    SDL_RenderPresent(appContext.ren);
}


void Menu::renderText(const std::string& text, int x, int y, bool selected) {
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Solid(appContext.font, text.c_str(), white);
    if (!surface) {
        SDL_Log("TTF_RenderText_Solid error: %s", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(appContext.ren, surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface error: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    int text_width, text_height;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

    SDL_Rect dst = {x, y, text_width, text_height};
    SDL_RenderCopy(appContext.ren, texture, nullptr, &dst);

    if (selected) {
        SDL_SetRenderDrawColor(appContext.ren, 255, 255, 255, 255);
        SDL_Rect outline = {x - 10, y - 5, text_width + 20, text_height + 10};
        SDL_RenderDrawRect(appContext.ren, &outline);
        SDL_RenderDrawRect(appContext.ren, &outline); // опциональная вторая линия
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}


void Menu::show_popup(std::string text){

    SDL_Renderer* ren = appContext.ren;
    TTF_Font* font = appContext.font;
    int screen_width = appContext.width;
    int screen_height = appContext.height;

    int win_w = screen_width * 0.6;
    int win_h = screen_height * 0.3;
    int win_x = screen_width / 2 - win_w / 2;
    int win_y = screen_height / 4;

    SDL_Rect message_box = {win_x, win_y, win_w, win_h};
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 150);
    SDL_RenderFillRect(ren, &message_box);
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface* surface1 = TTF_RenderUTF8_Blended(font, text.c_str(), white);
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(ren, surface1);

    int w1, h1;
    SDL_QueryTexture(texture1, nullptr, nullptr, &w1, &h1);

    int spacing = 10;
    int totalHeight = h1 + spacing;

    SDL_Rect dst1 = {
            win_x + win_w / 2 - w1 / 2,
            win_y + win_h / 2 - totalHeight / 2,
            w1, h1
    };

    SDL_RenderCopy(ren, texture1, nullptr, &dst1);

    SDL_RenderPresent(ren);

    SDL_FreeSurface(surface1);
    SDL_DestroyTexture(texture1);

}