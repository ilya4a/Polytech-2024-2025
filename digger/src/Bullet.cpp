#include "Bullet.h"

Bullet::Bullet(TileMap &tile_map, int x, int y, int width, int height, double v) : GameObject(tile_map, x, y, width,
                                                                                              height, v) {
    can_update_direction = true;
    alive = false;
}

void Bullet::update(double delta_time) {

    apply_object_in_tile_action();
    if(alive){
        if(can_update_direction){
            direction = tile_map.player_direction;
            x = tile_map.player_x;
            y = tile_map.player_y;
            can_update_direction = false;
        }
        double delta = v * delta_time;
        if(direction == Direction::DOWN) y += delta;
        if(direction == Direction::UP) y -= delta;
        if(direction == Direction::RIGHT) x += delta;
        if(direction == Direction::LEFT) x-= delta;

        if(handle_board_collision(delta_time)){
            alive = false;
        }
    }else{
        can_update_direction = true;
        x = tile_map.player_x;
        y = tile_map.player_y;
    }
}

void Bullet::draw(AppContext &app_context) {
    if(alive){
        SDL_SetRenderDrawColor(app_context.ren, 255, 255, 255, 255);
        SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), width, height};
        SDL_RenderFillRect(app_context.ren, &rect);
    }
}

void Bullet::object_in_tile_action(Tile &tile) {
    if(tile.type == TileType::Dirt){
        alive = false;
    }
}


