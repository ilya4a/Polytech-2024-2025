#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <string>

#include "AppContext.h"
#include "Constants.h"
#include "Tile.h"

using namespace std;

class TileMap {
    int map_line_row, map_line_col;
    int width, height;
    vector<pair<int, int>> diamond_coordinates;
    pair<int, int> playerSpawn;
    pair<int, int> enemySpawn;
    int tile_size;
    int rows, cols;
    SDL_Texture *diamond_texture;
    void init_tiles(vector<string> raw_lines);
    bool is_win;
public:
    bool all_diamond_collected();
    int get_cols() const;

    int get_rows() const;

    int get_tile_size() const;

    int get_width() const;

    int get_height() const;

    void init_diamond_texture(AppContext &app_context);

    pair<int, int> get_playerSpawn();

    pair<int, int> get_enemySpawn();

    int player_x = 0, player_y = 0;
    Direction player_direction;
    vector<vector<Tile>> tiles;

    int handle_diamonds_with_player();

    TileMap(const std::string &path, int screen_width, int screen_height);

    ~TileMap();

    void draw(const AppContext &app_context);
};


#endif //TILEMAP_H
