cmake_minimum_required(VERSION 3.10)
project(digger)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)

add_executable(digger
        src/main.cpp
        src/Game.cpp
        src/TileMap.cpp
        src/Tile.cpp
        src/GameObject.cpp
        src/GameManager.cpp
        src/Player.cpp
        src/AppContext.cpp
        src/Alien.cpp
        src/Menu.cpp
        src/Bullet.cpp
)

add_custom_command(TARGET digger POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:digger>/assets
)

target_include_directories(digger PRIVATE include)

target_link_libraries(digger
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        SDL2_image::SDL2_image
        nlohmann_json::nlohmann_json
)

