#ifndef ALIEN_H
#define ALIEN_H
#include "GameObject.h"

class Alien : public GameObject{


    static constexpr double BASE_PRIORITY_1 = 1.0;
    static constexpr double AXIS_BONUS_1 = 1.7;
    static constexpr double LINE_BONUS_1 = 1.5;
    static constexpr double LINE_BONUS_EXTRA_1 = 2;
    static constexpr double HISTORY_PENALTY_1 = 0.85;
    static constexpr double OSCILLATION_PENALTY_1 = 0.3;
    static constexpr double AXIS_FINDING_COF_1 = 2;

    static constexpr double BASE_PRIORITY_2 = 1.0;
    static constexpr double AXIS_BONUS_2 = 1.5;
    static constexpr double LINE_BONUS_2 = 2;
    static constexpr double LINE_BONUS_EXTRA_2 = 4;
    static constexpr double HISTORY_PENALTY_2 = 0.85;
    static constexpr double OSCILLATION_PENALTY_2 = 0.6;
    static constexpr double AXIS_FINDING_COF_2 = 4;

    static constexpr double BASE_PRIORITY_3 = 1.0;
    static constexpr double AXIS_BONUS_3 = 1.5;
    static constexpr double LINE_BONUS_3 = 0.7;
    static constexpr double LINE_BONUS_EXTRA_3 = 1.5;
    static constexpr double HISTORY_PENALTY_3 = 0.85;
    static constexpr double OSCILLATION_PENALTY_3 = 0.4;
    static constexpr double AXIS_FINDING_COF_3 = 1;


    double base_priority;
    double axis_bonus;
    double line_bonus;
    double line_bonus_extra;
    double history_penalty;
    double oscillation_penalty;
    double axis_finding_cof;

    int player_x, player_y;
    int steps_to_think;
    int max_steps_to_think;
    bool is_line_moving = true;
    vector<Direction> last_4_directions;
    vector<Direction> last_8_directions;


    pair<double, double> get_new_coordinates_dy_direction(double delta_time, int tiles_in_current_direction);
    void change_max_step();
    void handle_support_vectors();
    vector<pair<Direction, double>> calculate_scores(vector<Direction> available_directions);
    Direction choose_player_direction(vector<Direction> available_directions);
    int available_tiles_in_direction(Direction direction);
    vector<Direction> get_available_directions();

    public:
    static int alien_order;
    Alien(TileMap &tile_map, int x, int y, int width, int height, double v);
    void update(double delta_time) override;

    static void reset_order();
};



#endif //ALIEN_H
