#pragma once

#include <atomic>

constexpr uint8_t g_canvas_size_x = 68;
constexpr uint8_t g_canvas_size_y = 30;

// Currently no way to change the magnitude of the components.
struct Vector {
    int8_t x = 2;
    int8_t y = 1;
};

struct Position {
    uint8_t x = 0;
    uint8_t y = 0;
};

struct Ball {
    Vector velocity;
    Position position;

    void update();
};

class Game {
public:
    Game();
    virtual ~Game();
    virtual void run(std::atomic<bool>& game_should_run) = 0;
};

class DerivedGame : public Game {
public:
    void run(std::atomic<bool>& game_should_run) override;

    void draw() const;

private:
    Ball ball;
};

extern "C" Game* make_game();
extern "C" void destroy_game(Game* game);
