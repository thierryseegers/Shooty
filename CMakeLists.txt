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
                OPTIONS
                    tomlplusplus:multiple_headers=True
)
list(APPEND CMAKE_MODULE_PATH ${CONAN_INSTALL_FOLDER})

find_package(magic_enum 0.7.2 MODULE
    REQUIRED
)

find_package(sfml 2.5.1 MODULE
    COMPONENTS audio graphics network system window
    REQUIRED
)

find_package(spdlog 1.8.2 MODULE
    REQUIRED
)

find_package(tomlplusplus 2.3.0 MODULE
    REQUIRED
)

add_executable(Shooty
    application.cpp
    application.h
    command.h
    configuration.cpp
    configuration.h
    effects/bloom.cpp
    effects/bloom.h
    effects/post_effect.h
    entity/aircraft.cpp
    entity/aircraft.h
    entity/animation.h
    entity/bullet.cpp
    entity/bullet.h
    entity/enemy.cpp
    entity/enemy.h
    entity/entity.h
    entity/explosion.h
    entity/leader.cpp
    entity/leader.h
    entity/missile.cpp
    entity/missile.h
    entity/pickup.cpp
    entity/pickup.h
    entity/projectile.cpp
    entity/projectile.h
    gui/button.cpp
    gui/button.h
    gui/component.cpp
    gui/component.h
    gui/container.cpp
    gui/container.h
    gui/label.cpp
    gui/label.h
    lifebar.cpp
    lifebar.h
    main.cpp
    music.cpp
    music.h
    particle.cpp
    particle.h
    player.cpp
    player.h
    resources.cpp
    resources.h
    scene.cpp
    scene.h
    sound.cpp
    sound.h
    state/game_over.cpp
    state/game_over.h
    state/game.cpp
    state/game.h
    state/id.h
    state/menu.cpp
    state/menu.h
    state/pause.cpp
    state/pause.h
    state/settings.cpp
    state/settings.h
    state/stack.cpp
    state/stack.h
    state/state.cpp
    state/state.h
    state/title.cpp
    state/title.h
    utility.cpp
    utility.h
    world.cpp
    world.h
)

target_compile_options(Shooty
    PRIVATE -Wall
    PRIVATE -Wextra
    PRIVATE -Werror
    PRIVATE -pedantic
)

target_include_directories(Shooty
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE magic_enum::magic_enum
    PRIVATE sfml::sfml
    PRIVATE spdlog::spdlog
    PRIVATE tomlplusplus::tomlplusplus
)

target_link_libraries(Shooty
    PRIVATE sfml::sfml
    PRIVATE spdlog::spdlog
)

set_property(
    TARGET Shooty
    PROPERTY CXX_STANDARD 17
)

# Copy media files from the book's chapters and the configuration file.
add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Book/09_Audio/Media ${CMAKE_BINARY_DIR}/Media
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config.toml ${CMAKE_BINARY_DIR}/config.toml
)

add_dependencies(
    Shooty
    copy-runtime-files
)