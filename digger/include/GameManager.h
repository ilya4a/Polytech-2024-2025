#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameObject.h"
#include "TileMap.h"


class GameManager {

    std::unique_ptr<GameObject> player;
    std::unique_ptr<GameObject> bullet;

    double time_running;
    int score;
    double bullet_recharge;

    TileMap tileMap;
    std::vector<std::unique_ptr<GameObject>> aliens;
    void spawn_one_alien();
    bool check_player_loose();
    bool check_objects_intersection(GameObject &obj1, GameObject &obj2);
    int get_killed_alien();
    void handle_bullet_activate(double delta_time);
    AppContext &app_context;


public:
    bool is_win();
    int get_score() const;
    bool update(double delta_time);
    void draw(AppContext  &app_context);
    explicit GameManager(AppContext &app_context);
    ~GameManager() = default;
};



#endif //GAMEMANAGER_H
