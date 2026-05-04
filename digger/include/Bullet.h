#ifndef DIGGER_BULLET_H
#define DIGGER_BULLET_H


#include "GameObject.h"

class Bullet : public GameObject{
    bool can_update_direction;
    void object_in_tile_action(Tile &tile) override;
public:
    Bullet(TileMap&tile_map, int x, int y, int width, int height, double v);
    void update(double delta_time) override;
    void draw(AppContext &app_context) override;

};


#endif //DIGGER_BULLET_H
