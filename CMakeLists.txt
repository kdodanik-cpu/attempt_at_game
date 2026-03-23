cmake_minimum_required(VERSION 3.20)
project (attempt_at_game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure to build executable with all source files in directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(attempt_at_game ${SOURCES}
        src/core/GameLoop.h
        src/core/GameLoop.cpp
        src/player/Player.h
        src/player/Player.cpp
        src/world/World.cpp
        src/world/World.h)

# Include dirs
target_include_directories(attempt_at_game PRIVATE
    src
    imported_libraries/raylib/include
)

# Link static raylib lib
target_link_libraries(
attempt_at_game PRIVATE
${CMAKE_SOURCE_DIR}/imported_libraries/raylib/lib/raylib.lib
winmm)