#include "game/game.hpp"

#include <iostream>
#include <thread>

Game::Game()
{
    std::cout << "Game constructed\n";
}

Game::~Game()
{
    std::cout << "Game destroyed\n";
}

void DerivedGame::run()
{
    std::cout << "Hello from game on " << std::this_thread::get_id() << '\n';
}

Game* make_game()
{
    return new DerivedGame{};
}

void destroy_game(Game* game)
{
    delete game;
}
