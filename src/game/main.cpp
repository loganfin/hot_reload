// This file will be conditionally included from the build system when we're
// building with Release.
#include "game/game.hpp"

#include <memory>

int main()
{
    auto game = std::unique_ptr<Game, void (*)(Game*)>(
        game::make_game(), game::destroy_game
    );

    game->run();
}
