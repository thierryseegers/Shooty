cmake_minimum_required(VERSION 3.0.0)
project(Shooty VERSION 1.0.0)

# Get Conan.
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake"
        TLS_VERIFY ON)
endif()
include(
    ${CMAKE_BINARY_DIR}/conan.cmake
)

# Request Conan to install Find*.cmake modules for the dependencies.
set(CONAN_INSTALL_FOLDER ${CMAKE_BINARY_DIR}/conan)
conan_cmake_run(REQUIRES
                    magic_enum/0.7.2
                    tomlplusplus/2.3.0
                    sfml/2.5.1@bincrafters/stable
                    spdlog/1.8.2
                BASIC_SETUP
                BUILD
                    missing
                GENERATORS
                    cmake_find_package
                INSTALL_FOLDER
                    ${CONAN_INSTALL_FOLDER}
)
list(APPEND CMAKE_MODULE_PATH ${CONAN_INSTALL_FOLDER})

find_package(magic_enum 0.7.2 MODULE
    REQUIRED
)

find_package(sfml 2.5.1 MODULE
    COMPONENTS audio graphics network system window
    REQUIRED
)
add_library(sfml::sfml ALIAS SFML::SFML)

find_package(spdlog 1.8.2 MODULE
    REQUIRED
)

find_package(tomlplusplus 2.3.0 MODULE
    REQUIRED
)

add_executable(Shooty
    src/action.h
    src/application.cpp
    src/application.h
    src/bindings.cpp
    src/bindings.h
    src/command.h
    src/configuration.cpp
    src/configuration.h
    src/effects/bloom.cpp
    src/effects/bloom.h
    src/effects/post_effect.h
    src/entity/aircraft.cpp
    src/entity/aircraft.h
    src/entity/animation.h
    src/entity/bullet.cpp
    src/entity/bullet.h
    src/entity/enemy.cpp
    src/entity/enemy.h
    src/entity/entity.h
    src/entity/explosion.h
    src/entity/leader.cpp
    src/entity/leader.h
    src/entity/missile.cpp
    src/entity/missile.h
    src/entity/pickup.cpp
    src/entity/pickup.h
    src/entity/projectile.cpp
    src/entity/projectile.h
    src/gui/button.cpp
    src/gui/button.h
    src/gui/component.cpp
    src/gui/component.h
    src/gui/container.cpp
    src/gui/container.h
    src/gui/label.cpp
    src/gui/label.h
    src/lifebar.cpp
    src/lifebar.h
    src/main.cpp
    src/music.cpp
    src/music.h
    src/particle.cpp
    src/particle.h
    src/player.cpp
    src/player.h
    src/resources.cpp
    src/resources.h
    src/scene.cpp
    src/scene.h
    src/sound.cpp
    src/sound.h
    src/state/game_over.cpp
    src/state/game_over.h
    src/state/game.cpp
    src/state/game.h
    src/state/id.h
    src/state/menu.cpp
    src/state/menu.h
    src/state/pause.cpp
    src/state/pause.h
    src/state/settings.cpp
    src/state/settings.h
    src/state/stack.cpp
    src/state/stack.h
    src/state/state.cpp
    src/state/state.h
    src/state/title.cpp
    src/state/title.h
    src/utility.cpp
    src/utility.h
    src/world.cpp
    src/world.h
)

target_compile_options(Shooty
    PRIVATE -Wall
    PRIVATE -Wextra
    PRIVATE -Werror
    PRIVATE -pedantic
)

target_include_directories(Shooty
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(Shooty
    PRIVATE magic_enum::magic_enum
    PRIVATE sfml::sfml
    PRIVATE spdlog::spdlog
    PRIVATE tomlplusplus::tomlplusplus
)

set_property(
    TARGET Shooty
    PROPERTY CXX_STANDARD 17
)

# Copy media files from the book's chapters and the configuration file.
add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Media ${CMAKE_BINARY_DIR}/Media
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config.toml ${CMAKE_BINARY_DIR}/config.toml
)

add_dependencies(
    Shooty
    copy-runtime-files
)