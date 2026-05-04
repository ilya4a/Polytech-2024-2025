#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr int N_TILES_IN_SQUARE = 10;
const int N_TILES_IN_PLAYER = 6;
const int N_TILES_IN_BULLET = 3;

constexpr double COEF = (15.0 + 2.0/N_TILES_IN_SQUARE) / (10.0 + 2.0/N_TILES_IN_SQUARE);
constexpr int SCREEN_HEIGHT = 530;
constexpr int SCREEN_WIDTH = static_cast<int>(SCREEN_HEIGHT * COEF);
const int N_TILES_IN_DIAMOND = 8;

const int ADD_SEARCH_TILES_IN_DELETE = 2;

const double PLAYER_SPEED = 0.2;
const double ALIEN_SPEED = 0.4;
const double BULLET_SPEED = 0.5;

const int DELAY = 16;
const int NUM_OF_ALIENS_ON_START = 3;
const double ALIEN_SPAWN_TIME = 7000;

const double BULLET_RECHARGE_TIME = 1000;

enum class Direction {
    UP, DOWN, LEFT, RIGHT, STAY
};

enum class TileType {
    Empty,
    Dirt,
    Wall
};


#endif //CONSTANTS_H
