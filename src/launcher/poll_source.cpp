#include "launcher/poll_source.hpp"
#include "game/game.hpp"

#include <atomic>
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

namespace {

void* game_handle = nullptr;
// This is how we wait on the game thread to exit before starting a new one
auto game_thread_running = std::atomic<bool>{false};
// This signals the game thread to start and stop
auto game_thread_should_run = std::atomic<bool>{true};
// This signals the poll thread that the game thread has been loaded
auto poll_thread_should_run = std::atomic<bool>{true};

// These would ideally be passed in from CMake as macros
constexpr auto source_dir = "./src/game";
constexpr auto binary_path = "./build/libgame.so";

bool should_recompile();
void load_library();
void print_hello();

} // namespace

void poll_source()
{
    // This condition will be false if there was an error in load_library,
    // causing the program to exit.
    while (game_thread_should_run.load()) {
        if (!should_recompile()) {
            continue;
        }

        std::cerr << "Source code modified...\n";

        // Sometimes the lib won't be updated if we write the file without
        // making a change, which cause the compiler to be called over and over.
        // Should we move the file first? Would that take too long as the game
        // binary increases in size?

        // I don't know how stuttery it will be once there's graphics involved.
        // Maybe we could double-buffer the game libraries, swapping them once
        // the new one is fully loaded.

        // Recompile. Is there a more portable way to do this?
        if (system("cmake --build build -t game") != 0) {
            std::cerr << "Compile failed\n";
            continue;
        }

        load_library();
    }
}

namespace {

bool should_recompile()
{
    // Return true on the first call so the game starts without us making a
    // change to the source code.
    static bool first_call = true;

    if (first_call) {
        first_call = false;
        return true;
    }

    // We could alternatively use inotify, but that's Linux specific and not
    // portable. Do I care?

    // The directory iterator may be invalid if we change the file while it's
    // iterating, but just ignore it.
    try {
        for (auto const& file :
             std::filesystem::recursive_directory_iterator(source_dir)) {
            if (std::filesystem::last_write_time(file) >
                std::filesystem::last_write_time(binary_path)) {
                return true;
            }
        }
    } catch (std::filesystem::filesystem_error const&) {
    }

    return false;
}

void load_library()
{
    // Signal the game thread to close if it's running
    game_thread_should_run.store(false);

    // Wait for the thread to close. Should we be using a mutex so that we're
    // not busy waiting?
    while (game_thread_running.load()) {
        ;
    }

    // It's safe to unload the old library now.
    if (game_handle) {
        dlclose(game_handle);
        game_handle = nullptr;
    }

    game_handle = dlopen(binary_path, RTLD_LAZY);

    if (!game_handle) {
        std::cerr << "Failed to link " << binary_path << '\n';
        return;
    }

    auto* make_game =
        reinterpret_cast<Game* (*)()>(dlsym(game_handle, "make_game"));
    auto* destroy_game =
        reinterpret_cast<void (*)(Game*)>(dlsym(game_handle, "destroy_game"));

    if (!make_game || !destroy_game) {
        std::cerr << "Failed to load functions from " << binary_path << "\n";
        dlclose(game_handle);
        game_handle = nullptr;
        return;
    }

    // This method will require the event loop to check for the state of the
    // value... I would like a less intrusive way, but let's try this for now.
    // Maybe not so bad since we're probably also going to want to use this
    // variable to check if other conditions would require us to exit the game
    // like the window's close button being called.

    game_thread_should_run.store(true);

    std::thread{[make_game, destroy_game]() {
        auto game =
            std::unique_ptr<Game, void (*)(Game*)>(make_game(), destroy_game);

        game_thread_running.store(true);

        while (game_thread_should_run.load()) {
            // print_hello(thread_since_start);
            game->run();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "thread exiting...\n";
        game_thread_running.store(false);
    }}.detach();
}

} // namespace
