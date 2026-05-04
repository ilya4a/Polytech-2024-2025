#include <memory>
#include <iostream>
#include "GameManager.h"

#include "Alien.h"
#include "Player.h"
#include "Bullet.h"

using namespace std;

void GameManager::spawn_one_alien() {
    aliens.push_back(
            make_unique<Alien>(
           tileMap,
           tileMap.get_enemySpawn().first,
           tileMap.get_enemySpawn().second,
           N_TILES_IN_PLAYER * tileMap.get_tile_size(),
           N_TILES_IN_PLAYER * tileMap.get_tile_size(),
           ALIEN_SPEED
       )
   );
    aliens.back()->init_texture(app_context);
}

bool GameManager::check_player_loose() {
 for (int i = 0; i < aliens.size(); i++) {
        if(check_objects_intersection(*aliens[i], *player)){
            return true;
        }
    }
    return false;
}

bool GameManager::check_objects_intersection(GameObject &obj1, GameObject &obj2) {
    double obj1_x = obj1.get_x();
    double obj1_y = obj1.get_y();
    double obj1_width =  obj1.get_width();
    double obj1_height = obj1.get_height();

    double obj2_x = obj2.get_x();
    double obj2_y = obj2.get_y();
    double obj2_width =  obj2.get_width();
    double obj2_height = obj2.get_height();

    if(obj1_x < obj2_x + obj2_width &&
            obj1_x + obj1_width > obj2_x &&
            obj1_y < obj2_y + obj2_height &&
            obj1_y + obj1_height > obj2_y){
        return true;
    }
    return false;
}


int GameManager::get_killed_alien() {
    if(!bullet->alive) return -1;

    for (int i = 0; i < aliens.size(); i++) {
        if(check_objects_intersection(*aliens[i], *bullet)){
            return i;
        }
    }
    return -1;
}


void GameManager::handle_bullet_activate(double delta_time) {
    bullet_recharge += delta_time;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_SPACE] && bullet_recharge >= BULLET_RECHARGE_TIME){
        bullet_recharge = 0;
        bullet->alive = true;
    }
}

bool GameManager::update(double delta_time) {

    handle_bullet_activate(delta_time);

    time_running += delta_time;
    if(static_cast<int>(time_running/ALIEN_SPAWN_TIME) > (Alien::alien_order - NUM_OF_ALIENS_ON_START + 1)){
        spawn_one_alien();
    }

    bullet->update(delta_time);
    player->update(delta_time);

    int killed = get_killed_alien();
    if(killed >= 0) {
        aliens.erase(aliens.begin() + killed);
        bullet->alive = false;
    }

    for(auto& alien : aliens ){
        alien->update(delta_time);
    }

    return  check_player_loose();
}

void GameManager::draw(AppContext &app_context) {
    SDL_SetRenderDrawColor(app_context.ren, 0, 0, 0, 255);
    SDL_RenderClear(app_context.ren);
    score += tileMap.handle_diamonds_with_player();

    tileMap.draw(app_context);
    player->draw(app_context);
    bullet->draw(app_context);

    for(auto& alien : aliens ){
        alien->draw(app_context);
    }

}

GameManager::GameManager(AppContext &app_context) :
tileMap("map.txt", SCREEN_WIDTH, SCREEN_HEIGHT),
    app_context(app_context) {

    tileMap.init_diamond_texture(app_context);

    for(int i = 0; i < NUM_OF_ALIENS_ON_START - 1; i++){
        spawn_one_alien();
    }
    int tile_size = tileMap.get_tile_size();
    player = make_unique<Player>(tileMap, tileMap.get_playerSpawn().first, tileMap.get_playerSpawn().second, (N_TILES_IN_PLAYER)*tile_size, (N_TILES_IN_PLAYER)*tile_size,
                                      PLAYER_SPEED);
    player->init_texture(app_context);

    bullet = make_unique<Bullet>(tileMap, tileMap.get_playerSpawn().first, tileMap.get_playerSpawn().second, (N_TILES_IN_BULLET)*tile_size, (N_TILES_IN_BULLET)*tile_size,
                                 BULLET_SPEED);
    time_running = 0;
    score = 0;
    time_running = 0;
    bullet_recharge = 0;
}

int GameManager::get_score() const {
    return score;
}

bool GameManager::is_win() {
    return tileMap.all_diamond_collected();
}


