#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "AppContext.h"
#include "Constants.h"
#include "TileMap.h"


class GameObject {

protected:
    double v = 0;
    TileMap &tile_map;
    bool handle_board_collision(double delta);
    double x, y;
    int width, height;
    Direction direction;
    void apply_object_in_tile_action();
    SDL_Texture* texture;
    string picture_name;

public:
    bool alive;
    double get_x() const;
    double get_y() const;
    double get_width() const;
    double get_height() const;
    void init_texture(AppContext &app_context);
    virtual void update(double delta_time) = 0;
    virtual void draw(AppContext &app_context);
    virtual void object_in_tile_action(Tile &tile);
    GameObject(TileMap &tile_map, int x, int y, int width, int height, double v);
    virtual ~GameObject();
};



#endif //GAMEOBJECT_H
