cmake_minimum_required(VERSION 3.0.0)
project(Shooty VERSION 1.0.0)

include(CTest)
enable_testing()

find_package(SFML 2.5
    COMPONENTS graphics audio
    REQUIRED
)

add_executable(Shooty
    aircraft.cpp
    aircraft.h
    application.cpp
    application.h
    command.h
    entity.h
    gui/button.cpp
    gui/button.h
    gui/component.cpp
    gui/component.h
    gui/container.cpp
    gui/container.h
    gui/label.cpp
    gui/label.h
    main.cpp
    player.cpp
    player.h
    resources.h
    scene.cpp
    scene.h
    state.cpp
    state.h
    utility.cpp
    utility.h
    world.cpp
    world.h
)

target_include_directories(Shooty
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE /usr/local/include
)

target_link_libraries(Shooty
    sfml-graphics
)

set_property(TARGET Shooty
    PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
