#include <SDL_image.h>
#include "GameObject.h"

bool GameObject::handle_board_collision(double delta) {
    if (x <= 0){ x += delta; return true;}
    if (x + width >= tile_map.get_width()) {x -= delta; return true;}

    if (y <= 0) {y += delta; return true;}
    if (y + height >= tile_map.get_height()){ y -= delta; return true;}
    return false;
}

GameObject::GameObject(TileMap &tile_map, int x, int y, int width, int height, double v)
    : tile_map(tile_map), x(x), y(y), width(width), height(height), v(v)
{
    direction = Direction::STAY;
    alive = true;
    picture_name = "GameObject.png";
}

void GameObject::init_texture(AppContext &app_context) {
    string path = "assets/pictures/" + picture_name;
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
    }

    texture = SDL_CreateTextureFromSurface(app_context.ren, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}


void GameObject::apply_object_in_tile_action() {
    int tile_size = tile_map.get_tile_size();

    int cols_start = static_cast<int>(x - ADD_SEARCH_TILES_IN_DELETE*tile_size) / tile_size;
    if (cols_start < 0) cols_start = 0;

    int cols_end = static_cast<int>(x + width + ADD_SEARCH_TILES_IN_DELETE*tile_size) / tile_size;
    if (cols_end >= tile_map.get_cols()) cols_end = tile_map.get_cols() - 1;

    int rows_start = static_cast<int>(y - ADD_SEARCH_TILES_IN_DELETE*tile_size) / tile_size;
    if (rows_start < 0) rows_start = 0;

    int rows_end = static_cast<int>(y + height + ADD_SEARCH_TILES_IN_DELETE*tile_size) / tile_size;
    if (rows_end >= tile_map.get_rows()) rows_end = tile_map.get_rows() - 1;

    for (int col = cols_start; col <= cols_end; col++) {
        for (int row = rows_start; row <= rows_end; row++) {
            Tile &tile = tile_map.tiles[row][col];
            int tile_x = col * tile_size;
            int tile_y = row * tile_size;

            if (x  - tile_size< tile_x + tile_size &&
                x + width + tile_size> tile_x &&
                y  - tile_size < tile_y + tile_size &&
                y + height  + tile_size > tile_y){
                    if (tile.type != TileType::Wall) {
                        object_in_tile_action(tile);
                    }

            }
        }
    }
}

double GameObject::get_x() const {
    return x;
}

double GameObject::get_y() const {
    return y;
}

double GameObject::get_width() const {
    return width;
}

double GameObject::get_height() const {
    return height;
}

void GameObject::object_in_tile_action(Tile &tile) {

}

GameObject::~GameObject() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void GameObject::draw(AppContext &app_context) {
    SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), width, height};
    SDL_RenderCopy(app_context.ren, texture, nullptr, &rect);
}
