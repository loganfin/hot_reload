#pragma once

class Game {
public:
    Game();
    virtual ~Game();
    virtual void run() = 0;
};

class DerivedGame : public Game {
public:
    void run() override;
};

extern "C" Game* make_game();
extern "C" void destroy_game(Game* game);
