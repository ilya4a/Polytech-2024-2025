#include "TileMap.h"
#include <fstream>
#include <iostream>
#include <SDL_image.h>

#include "AppContext.h"

using namespace std;

vector<string> read_map(const string& path) {
    vector<string> raw_lines;
    string line;
    int expected_width = 0;

    ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR read_map" << std::endl;
    }
    while (getline(file, line)) {
        if (expected_width == 0) expected_width = line.size();
        else if (line.size() != expected_width) {

            std::cout << "\n ERROR read_map" <<std::endl;
        }
        raw_lines.push_back(line);
    }
    file.close();
    return raw_lines;
}

TileMap::TileMap(const string& path, int screen_width, int screen_height) {

    vector<string> raw_lines = read_map(path);

    map_line_row = raw_lines.size();
    map_line_col = raw_lines[0].size();

    rows = (map_line_row - 2) * N_TILES_IN_SQUARE + 2;
    cols = (map_line_col - 2) * N_TILES_IN_SQUARE + 2;

    tile_size = std::min(
       screen_width / cols,
       screen_height / rows
   );

    width = (cols) * tile_size;
    height = (rows) * tile_size;


    tiles.resize(rows, vector<Tile>(cols, Tile(TileType::Empty, 0, 0)));
    init_tiles(raw_lines);
    player_direction = Direction::STAY;
    is_win= false;
}
void getRandomBrown(int& r, int& g, int& b) {
    r = 150 + (rand() % 61 - 30);
    g = 75 + (rand() % 61 - 30);
    b = rand() % 31;
}

void TileMap::init_tiles(vector<string> raw_lines) {
    int r =0; int g = 0; int b = 0;
    for (int i = 1; i < map_line_row - 1; i++) {
        for (int j = 1; j < map_line_col - 1; j++) {
            char symbol = raw_lines[i][j];
            int base_y = (i - 1) * N_TILES_IN_SQUARE + 1;
            int base_x = (j - 1) * N_TILES_IN_SQUARE + 1;

            TileType new_tile_type = TileType::Empty;
            if (symbol == '=') {
                new_tile_type = TileType::Dirt;
            }
            if(symbol == 'd'){
                diamond_coordinates.emplace_back(base_x * tile_size, base_y*tile_size);
                new_tile_type = TileType::Dirt;
            }

            for (int dy = 0; dy < N_TILES_IN_SQUARE; dy++) {
                for (int dx = 0; dx < N_TILES_IN_SQUARE; dx++) {
                    tiles[base_y + dy][base_x + dx] =
                        Tile(new_tile_type, (base_x + dx) * tile_size, (base_y + dy) * tile_size);
                    if (new_tile_type == TileType::Dirt) {
                        getRandomBrown(r, g, b);
                        tiles[base_y + dy][base_x + dx].set_color(r, g, b);
                    }
                }
            }
            if (symbol == 'p') playerSpawn = {(base_x +1) * tile_size, (base_y + 1)* tile_size};
            if (symbol == 'e') enemySpawn = {(base_x +1) * tile_size, (base_y + 1)* tile_size};
        }
    }

    for (int y = 0; y < rows; y++) {
        tiles[y][0] = Tile(TileType::Wall, 0, y * tile_size);
        tiles[y][cols - 1] = Tile(TileType::Wall, (cols - 1) * tile_size, y * tile_size);
    }

    for (int x = 0; x < cols; x++) {
        tiles[0][x] = Tile(TileType::Wall, x * tile_size, 0);
        tiles[rows - 1][x] = Tile(TileType::Wall, x * tile_size, (rows - 1) * tile_size);
    }
}

int TileMap::handle_diamonds_with_player(){

    double player_width =  N_TILES_IN_PLAYER * tile_size;
    double player_height = N_TILES_IN_PLAYER * tile_size;

    int num_deleted = 0;
    for (int i = 0; i < diamond_coordinates.size(); i++){
        pair<int, int> diamond = diamond_coordinates[i];

        if(player_x < diamond.first + N_TILES_IN_DIAMOND*tile_size &&
           player_x + player_width > diamond.first &&
           player_y < diamond.second + N_TILES_IN_DIAMOND*tile_size &&
           player_y + player_height > diamond.second ){
            diamond_coordinates.erase(diamond_coordinates.begin() + i);
            num_deleted++;
        }
    }
    return num_deleted;
}

void TileMap::draw(const AppContext &app_context) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int x = tiles[i][j].x;
            int y = tiles[i][j].y;

            Tile current_tile = tiles[i][j];

            switch (current_tile.type) {
                case TileType::Dirt:
                    SDL_SetRenderDrawColor(app_context.ren, current_tile.r, current_tile.g, current_tile.b, 255);
                    break;
                case TileType::Wall:
                    SDL_SetRenderDrawColor(app_context.ren, 180, 20, 50, 200);
                    break;
                case TileType::Empty:
                    SDL_SetRenderDrawColor(app_context.ren, 0, 0, 0, 255);
                    break;
            }
            SDL_Rect rect = {x, y, tile_size, tile_size};
            SDL_RenderFillRect(app_context.ren, &rect);
        }

        SDL_SetRenderDrawColor(app_context.ren, 0, 255, 0, 255);
        SDL_Rect rect = {playerSpawn.first, playerSpawn.second, tile_size, tile_size};
        SDL_RenderFillRect(app_context.ren, &rect);

        rect = {enemySpawn.first, enemySpawn.second, tile_size, tile_size};
        SDL_RenderFillRect(app_context.ren, &rect);

    }

    if(diamond_coordinates.empty()) is_win = true;
    for(auto &diamond: diamond_coordinates){
        SDL_Rect rect = {static_cast<int>(diamond.first), static_cast<int>(diamond.second), N_TILES_IN_DIAMOND * tile_size, N_TILES_IN_DIAMOND* tile_size};
        SDL_RenderCopy(app_context.ren, diamond_texture, nullptr, &rect);
    }
}

int TileMap::get_width() const {
    return width;
}

int TileMap::get_height() const {
    return height;
}

pair<int, int> TileMap::get_playerSpawn() {
    return playerSpawn;
}

pair<int, int> TileMap::get_enemySpawn() {
    return enemySpawn;
}

int TileMap::get_tile_size() const {
    return tile_size;
}

int TileMap::get_cols() const {
    return cols;
}

int TileMap::get_rows() const {
    return rows;
}

void TileMap::init_diamond_texture(AppContext &app_context) {
    string path = "assets/pictures/diamond.png";
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        SDL_Log("Failed to load image: %s", IMG_GetError());
    }

    diamond_texture = SDL_CreateTextureFromSurface(app_context.ren, surface);
    SDL_FreeSurface(surface);

    if (!diamond_texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

TileMap::~TileMap() {
    if (diamond_texture) {
        SDL_DestroyTexture(diamond_texture);
        diamond_texture = nullptr;
    }
}

bool TileMap::all_diamond_collected() {
    return is_win;
}
