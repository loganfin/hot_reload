#include "game/game.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>

void Ball::update()
{
    // This can probably be improved
    if (velocity.x > 0) {
        if (position.x < g_canvas_size_x - velocity.x) {
            position.x += velocity.x;
        } else {
            velocity.x = bounce_off_wall(velocity.x);
        }
    } else {
        if (position.x > 0 - velocity.x - 1) {
            position.x += velocity.x;
        } else {
            velocity.x = bounce_off_wall(velocity.x);
        }
    }

    if (velocity.y > 0) {
        if (position.y < g_canvas_size_y - velocity.y) {
            position.y += velocity.y;
        } else {
            velocity.y = bounce_off_wall(velocity.y);
        }
    } else {
        if (position.y > 0 - velocity.y - 1) {
            position.y += velocity.y;
        } else {
            velocity.y = bounce_off_wall(velocity.y);
        }
    }
}

int32_t Ball::bounce_off_wall(int32_t vel_component)
{
    // You can mess around in here to change bounce behavior at run time
    int32_t ret_val = (vel_component * -1);
    // if (ret_val == 0) {
    //     return vel_component * -1;
    // }
    return ret_val;
}

Game::Game()
{
    srand(time(nullptr));
    std::cout << "Game constructed\n";
}

Game::~Game()
{
    std::cout << "Game destroyed\n";
}

DerivedGame::DerivedGame(GameData* game_data) : data_{game_data} {}

void DerivedGame::run(std::atomic<bool>& game_should_run)
{
    while (game_should_run.load()) {
        data_->ball.update();

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
            if (data_->ball.position.x == column &&
                data_->ball.position.y == row) {
                // Try changing the character used for the ball at runtime
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
    std::cout << "Current velocity: (" << data_->ball.velocity.x << ", "
              << data_->ball.velocity.y << ")\n";
    std::cout << "Current position: (" << data_->ball.position.x << ", "
              << data_->ball.position.y << ")\n";
}

Game* make_game(GameData* game_data)
{
    return new DerivedGame{game_data};
}

void destroy_game(Game* game)
{
    delete game;
}
