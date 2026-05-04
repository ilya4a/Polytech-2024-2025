#include "Alien.h"

#include <algorithm>
#include <iostream>

int Alien::alien_order = 0;

void Alien::reset_order() {
    alien_order = 0;
}

vector<pair<Direction, double>> Alien::calculate_scores(vector<Direction> available_directions) {
    if (available_directions.empty()) return {};

    player_x = tile_map.player_x;
    player_y = tile_map.player_y;

    double delta_x = player_x - x;
    double delta_y = player_y - y;

    double score_UP = 0;
    double score_DOWN = 0;
    double score_LEFT = 0;
    double score_RIGHT = 0;

    if (std::find(available_directions.begin(), available_directions.end(), Direction::LEFT) != available_directions.end()) score_LEFT = base_priority;
    if (std::find(available_directions.begin(), available_directions.end(), Direction::RIGHT) != available_directions.end()) score_RIGHT = base_priority;
    if (std::find(available_directions.begin(), available_directions.end(), Direction::UP) != available_directions.end()) score_UP = base_priority;
    if (std::find(available_directions.begin(), available_directions.end(), Direction::DOWN) != available_directions.end()) score_DOWN = base_priority;

    if (fabs(fabs(delta_x) - fabs(delta_y)) < width * axis_finding_cof ) {
        if (rand()%2) {
            if (delta_x > 0) score_RIGHT *= axis_bonus;
            else score_LEFT *= axis_bonus;
        }
        else {
            if (delta_y > 0) score_DOWN *= axis_bonus;
            else score_UP *= axis_bonus;
        }
    }else {
        if (fabs(delta_x) > fabs(delta_y)) {
            if (delta_x > 0) score_RIGHT *= axis_bonus;
            else score_LEFT *= axis_bonus;
        }else {
            if (delta_y > 0) score_DOWN *= axis_bonus;
            else score_UP *= axis_bonus;
        }
    }

    bool increase_line_moving_bonus = true;
    if (last_8_directions.size() == 8) {
        for (int i = 0; i < 4; i++) {
            if (last_8_directions[i] != last_8_directions[i + 4]) {
                increase_line_moving_bonus = false;
                is_line_moving = true;
                break;
            }
        }
        last_8_directions.clear();
    }

    double line_moving_bonus = line_bonus;
    if (increase_line_moving_bonus) line_moving_bonus = line_bonus_extra;

    switch (direction) {
        case Direction::UP: score_UP *= line_moving_bonus; break;
        case Direction::DOWN: score_DOWN *= line_moving_bonus; break;
        case Direction::LEFT: score_LEFT *= line_moving_bonus; break;
        case Direction::RIGHT: score_RIGHT *= line_moving_bonus; break;
        default: break;
    }

    double last_direction_penalty = history_penalty;

    if (last_4_directions.size() == 4) {
        if (last_4_directions[0] == last_4_directions[2]  && last_4_directions[1] == last_4_directions[3]) {
            if (last_4_directions[0] != last_4_directions[1]) {
                last_direction_penalty = oscillation_penalty;
            }
        }
    }

    for (int i = 0; i < last_4_directions.size(); ++i) {
        if (last_4_directions[i] == Direction::UP) score_UP *= last_direction_penalty;
        if (last_4_directions[i] == Direction::DOWN) score_DOWN *= last_direction_penalty;
        if (last_4_directions[i] == Direction::LEFT) score_LEFT *= last_direction_penalty;
        if (last_4_directions[i] == Direction::RIGHT) score_RIGHT *= last_direction_penalty;
    }
    vector<pair<Direction, double>> scores = {
            {Direction::UP, score_UP},
            {Direction::DOWN, score_DOWN},
            {Direction::LEFT, score_LEFT},
            {Direction::RIGHT, score_RIGHT}
    };
    return scores;
}


Direction Alien::choose_player_direction(vector<Direction> available_directions) {

    vector<pair<Direction, double>> scores = calculate_scores(available_directions);
    if(scores.empty()){
        return Direction::STAY;
    }
    sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    double max_score = scores.front().second;
    vector<Direction> best_directions;
    for (const auto& s : scores) {
        if (s.second == max_score && s.second > 0) {
            best_directions.push_back(s.first);
        }
    }
    if (!best_directions.empty()) {
        return best_directions[rand() % best_directions.size()];
    }
    Direction res = available_directions[rand() % available_directions.size()];
    return res;
}

int Alien::available_tiles_in_direction(Direction direction) {
    int tile_size = tile_map.get_tile_size();
    int max_steps = 0;
    while (true) {
        double new_x = x;
        double new_y = y;

        switch (direction) {
            case Direction::UP:
                new_y -= tile_size * (max_steps + 1);
            break;
            case Direction::DOWN:
                new_y += tile_size * (max_steps + 1);
            break;
            case Direction::LEFT:
                new_x -= tile_size * (max_steps + 1);
            break;
            case Direction::RIGHT:
                new_x += tile_size * (max_steps + 1);
            break;
        }

        int left = static_cast<int>(new_x) / tile_size;
        int right = static_cast<int>((new_x + width - 1)) / tile_size;
        int top = static_cast<int>(new_y) / tile_size;
        int bottom = static_cast<int>((new_y + height - 1)) / tile_size;

        bool can_move = true;

        for (int row = top; row <= bottom && can_move; ++row) {
            for (int col = left; col <= right && can_move; ++col) {
                if (row < 0 || row >= tile_map.get_rows() ||
                    col < 0 || col >= tile_map.get_cols() ||
                    tile_map.tiles[row][col].type != TileType::Empty
                    ) {
                    can_move = false;
                    }
            }
        }
        if (!can_move)
            break;

        max_steps++;
    }
    return max_steps;
}

vector<Direction> Alien::get_available_directions() {
    vector<Direction> moves;
    if (available_tiles_in_direction(Direction::UP) > 0) moves.push_back(Direction::UP);
    if (available_tiles_in_direction(Direction::DOWN) > 0) moves.push_back(Direction::DOWN);
    if (available_tiles_in_direction(Direction::LEFT) > 0) moves.push_back(Direction::LEFT);
    if (available_tiles_in_direction(Direction::RIGHT) > 0) moves.push_back(Direction::RIGHT);

    return moves;
}


pair<double, double> Alien::get_new_coordinates_dy_direction(double delta_time, int tiles_in_direction) {
    double new_x = x, new_y = y;

    double step = v * delta_time;
    if (step > tiles_in_direction) {
        step = tiles_in_direction;
    }

    switch (direction) {
        case Direction::UP:    new_y -= step; break;
        case Direction::DOWN:  new_y += step; break;
        case Direction::LEFT:  new_x -= step; break;
        case Direction::RIGHT: new_x += step; break;
    }
    return make_pair(new_x, new_y);
}

void Alien::change_max_step() {
    double delta_x = fabs(tile_map.player_x - x);
    double delta_y = fabs(tile_map.player_y - y);

    double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
    if (distance < width * 4) {
        max_steps_to_think = 1;
    }else {
        max_steps_to_think = 1 + static_cast<int>(distance/(N_TILES_IN_SQUARE * 2));
    }

    if (distance < width * 4) {
        v = ALIEN_SPEED * 0.5;
    }else {
        v = ALIEN_SPEED;
    }
}

void Alien::handle_support_vectors(){
    if (last_4_directions.size() >= 4) {
        last_4_directions.erase(last_4_directions.begin());
    }
    last_4_directions.push_back(direction);

    if (last_8_directions.size() >= 8) {
        last_8_directions.clear();
    }
    last_8_directions.push_back(direction);

}

void Alien::update(double delta_time) {

    change_max_step();
    int tiles_in_direction = 0;
    pair<double, double> coordinates = {0, 0};

    vector<Direction> moves = get_available_directions();
    tiles_in_direction = available_tiles_in_direction(direction);

    if (tiles_in_direction > 0  && (steps_to_think < max_steps_to_think || is_line_moving) ) {
        steps_to_think++;
        tiles_in_direction = available_tiles_in_direction(direction);
        coordinates = get_new_coordinates_dy_direction(delta_time, tiles_in_direction);
        if (tiles_in_direction > 0) {
            x = coordinates.first;
            y = coordinates.second;
        }else {
            std::cerr << "err update 1" << std::endl;
        }
    }else {
        is_line_moving = false;
        steps_to_think = 0;
        direction = choose_player_direction(moves);
        if(direction == Direction::STAY) return;
        tiles_in_direction = available_tiles_in_direction(direction);
        coordinates = get_new_coordinates_dy_direction(delta_time, tiles_in_direction);
        if (tiles_in_direction > 0) {
            x = coordinates.first;
            y = coordinates.second;
        }else {
            std::cerr << "err update 1" << std::endl;
        }
    }

    handle_support_vectors();
    handle_board_collision(v * delta_time);
}

Alien::Alien(TileMap &tile_map, int x, int y, int width, int height, double v) : GameObject(tile_map, x, y ,width, height, v) {
    player_x = 0;
    player_y = 0;
    steps_to_think = 0;
    last_4_directions = {};
    direction = Direction::LEFT;
    max_steps_to_think = 10;

    if(alien_order%2 == 0){
        base_priority = BASE_PRIORITY_1;
        axis_bonus = AXIS_BONUS_1;
        line_bonus = LINE_BONUS_1;
        line_bonus_extra = LINE_BONUS_EXTRA_1;
        history_penalty = HISTORY_PENALTY_1;
        oscillation_penalty = OSCILLATION_PENALTY_1;
        axis_finding_cof = AXIS_FINDING_COF_1;

    }else{
        base_priority = BASE_PRIORITY_2;
        axis_bonus = AXIS_BONUS_2;
        line_bonus = LINE_BONUS_2;
        line_bonus_extra = LINE_BONUS_EXTRA_2;
        history_penalty = HISTORY_PENALTY_2;
        oscillation_penalty = OSCILLATION_PENALTY_2;
        axis_finding_cof = AXIS_FINDING_COF_2;
    }
    alien_order++;
    picture_name = "alien.png";
}

