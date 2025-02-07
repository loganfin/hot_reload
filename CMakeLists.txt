cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(hot_reload LANGUAGES C CXX)

set(GAME_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/game/game.cpp
)

# If we're not building "Release", build the game as a shared library and load
# it dynamically with a launcher executable
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_executable(game
        ${CMAKE_CURRENT_SOURCE_DIR}/src/game/main.cpp
        ${GAME_SOURCES}
    )
else()
    add_library(game SHARED
        ${GAME_SOURCES}
    )

    add_executable(launcher
        ${CMAKE_CURRENT_SOURCE_DIR}/src/launcher/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/launcher/poll_source.cpp
    )

    target_include_directories(launcher
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endif()

target_include_directories(game
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

