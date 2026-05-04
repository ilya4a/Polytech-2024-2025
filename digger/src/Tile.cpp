#include "Tile.h"

void Tile::set_color(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Tile::Tile(TileType type, int x, int y) {
    this->type = type;
    this->x = x;
    this->y = y;
    r = 0; g = 0; b = 0;
}
