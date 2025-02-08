// This file will be conditionally included from the build system when we're
// building with Release.
#include "game/game.hpp"

#include <atomic>
#include <memory>

int main()
{
    std::atomic<bool> game_should_run{true};

    auto game_data = std::make_unique<GameData>();

    auto game = DerivedGame{game_data.get()};

    game.run(game_should_run);
}
