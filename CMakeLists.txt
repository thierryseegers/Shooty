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

# conan_cmake_run will create the find_package generators in ${CMAKE_BINARY_DIR}.
#  Make sure they will be found.
list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_BINARY_DIR}
)

conan_cmake_run(REQUIRES
                    tomlplusplus/2.3.0
                    sfml/2.5.1@bincrafters/stable
                BUILD
                    missing
                GENERATORS
                    cmake_find_package
                BASIC_SETUP
)

find_package(sfml 2.5.1
    COMPONENTS audio graphics network system window
    REQUIRED
)

find_package(tomlplusplus 2.3.0
    REQUIRED
)

add_executable(Shooty
    application.cpp
    application.h
    command.h
    configuration.cpp
    configuration.h
    entity/aircraft.cpp
    entity/aircraft.h
    entity/enemy.cpp
    entity/enemy.h
    entity/entity.h
    entity/leader.cpp
    entity/leader.h
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
    player.cpp
    player.h
    resources.h
    scene.cpp
    scene.h
    state/game.cpp
    state/game.h
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

target_include_directories(Shooty
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE sfml::sfml
    PRIVATE tomlplusplus::tomlplusplus
)

target_link_libraries(Shooty
    PRIVATE sfml::sfml
)

set_property(
    TARGET Shooty
    PROPERTY CXX_STANDARD 17
)

# Copy media files from the book's chapters.
add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Book/07_Gameplay/Media ${CMAKE_BINARY_DIR}/Media
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config.toml ${CMAKE_BINARY_DIR}/config.toml
)

add_dependencies(
    Shooty
    copy-runtime-files
)