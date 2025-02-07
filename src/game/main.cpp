// This file will be conditionally included from the build system when we're
// building with Release.
#include "game/game.hpp"

#include <atomic>
#include <memory>

int main()
{
    std::atomic<bool> game_should_run{true};

    auto game = DerivedGame{};

    game.run(game_should_run);
}
