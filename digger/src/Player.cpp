
#include "Player.h"
#include "AppContext.h"
#include "Constants.h"

Player::Player(TileMap &tile_map, int x, int y, int width, int height, double v) :
    GameObject(tile_map, x, y, width, height, v) {
    direction = Direction::STAY;
    picture_name = "player.png";
}

Player::~Player() = default;

double Player::change_coordinates(double delta_time) {
    double delta = v * delta_time;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP]) { y -= delta; direction = Direction::UP;}
    else if (state[SDL_SCANCODE_DOWN])  {y += delta; ; direction = Direction::DOWN;}
    else if (state[SDL_SCANCODE_LEFT])  {x -= delta; ; direction = Direction::LEFT;}
    else if (state[SDL_SCANCODE_RIGHT]) {x += delta; ; direction = Direction::RIGHT;}

    return delta;
}

void Player::object_in_tile_action(Tile &tile) {
    tile.set_color(0, 0, 0);
    tile.type = TileType::Empty;
}

void Player::update(double delta_time) {
    double delta = change_coordinates(delta_time);
    handle_board_collision(delta);
    apply_object_in_tile_action();

    tile_map.player_x = x;
    tile_map.player_y = y;
    tile_map.player_direction = direction;
}




