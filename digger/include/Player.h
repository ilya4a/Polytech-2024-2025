#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"

class Player : public GameObject{

    double change_coordinates(double delta_time);
    void object_in_tile_action(Tile &tile) override;

public:
    Player(TileMap&tile_map, int x, int y, int width, int height, double v);
    ~Player() override;

    void update(double delta_time) override;
};



#endif //PLAYER_H
