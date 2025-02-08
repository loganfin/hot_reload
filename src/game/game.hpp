#pragma once

#include <atomic>

constexpr uint32_t g_canvas_size_x = 69;
constexpr uint32_t g_canvas_size_y = 25;

struct Vector {
    int32_t x = 2;
    int32_t y = 1;
};

struct Position {
    uint32_t x = 0;
    uint32_t y = 0;
};

struct Ball {
    Vector velocity;
    Position position;

    void update();
    int32_t bounce_off_wall(int32_t vel_component);
};

class Game {
public:
    Game();
    virtual ~Game();
    virtual void run(std::atomic<bool>& game_should_run) = 0;
};

struct GameData {
    Ball ball;
};

class DerivedGame : public Game {
public:
    DerivedGame(GameData* data);

    void run(std::atomic<bool>& game_should_run) override;

    void draw() const;

private:
    GameData* data_ = nullptr;
};

extern "C" Game* make_game(GameData* game_data);
extern "C" void destroy_game(Game* game);
