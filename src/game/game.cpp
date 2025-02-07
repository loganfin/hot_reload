#include "game/game.hpp"

#include <iostream>
#include <thread>

void Ball::update()
{
    // This can probably be improved
    if (velocity.x > 0) {
        if (position.x < g_canvas_size_x - velocity.x) {
            position.x += velocity.x;
        } else {
            velocity.x *= -1;
        }
    } else {
        if (position.x > 0 - velocity.x - 1) {
            position.x += velocity.x;
        } else {
            velocity.x *= -1;
        }
    }

    if (velocity.y > 0) {
        if (position.y < g_canvas_size_y - velocity.y) {
            position.y += velocity.y;
        } else {
            velocity.y *= -1;
        }
    } else {
        if (position.y > 0 - velocity.y - 1) {
            position.y += velocity.y;
        } else {
            velocity.y *= -1;
        }
    }
}

Game::Game()
{
    std::cout << "Game constructed\n";
}

Game::~Game()
{
    std::cout << "Game destroyed\n";
}

void DerivedGame::run(std::atomic<bool>& game_should_run)
{
    while (game_should_run.load()) {
        ball.update();

        draw();

        // 30 "frames" per second
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}

void DerivedGame::draw() const
{
    // Clear the screen
    std::cout << "\x1B[2J\x1B[H";

    // Top bar
    for (int column = 0; column < g_canvas_size_x; column++) {
        if (column == 0) {
            std::cout << " _";
        } else if (column == g_canvas_size_x - 1) {
            std::cout << "_ ";
        } else {
            std::cout << "__";
        }
    }

    std::cout << '\n';

    // This won't look good in every terminal, but it's fine in mine.
    for (int row = 0; row < g_canvas_size_y; row++) {
        // Left side bar
        std::cout << "|";
        for (int column = 0; column < g_canvas_size_x; column++) {
            if (ball.position.x == column && ball.position.y == row) {
                std::cout << "🔴";
            } else if (column == g_canvas_size_x - 1) {
                std::cout << "";
            } else {
                std::cout << "  ";
            }
        }
        // Right side bar
        std::cout << "|\n";
    }

    // Bottom bar
    for (int column = 0; column < g_canvas_size_x; column++) {
        if (column == 0) {
            std::cout << " ‾";
        } else if (column == g_canvas_size_x - 1) {
            std::cout << "‾ ";
        } else {
            std::cout << "‾‾";
        }
    }
    std::cout << "\n";
}

Game* make_game()
{
    return new DerivedGame{};
}

void destroy_game(Game* game)
{
    delete game;
}
