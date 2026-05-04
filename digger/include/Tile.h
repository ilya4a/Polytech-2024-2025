#ifndef TILE_H
#define TILE_H
#include "Constants.h"


class Tile {
public:
    TileType type;
    int x;
    int y;

    int r, g, b;
    void set_color(int r, int g, int b);
    Tile(TileType type, int x, int y);
};



#endif //TILE_H
