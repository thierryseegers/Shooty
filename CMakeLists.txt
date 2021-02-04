cmake_minimum_required(VERSION 3.0.0)
project(Shooty VERSION 1.0.0)

include(CTest)
enable_testing()

find_package(SFML 2.5
    COMPONENTS graphics audio
    REQUIRED
)

add_executable(Shooty
    game.cpp
    game.h
    main.cpp
    resources.h
)

target_include_directories(Shooty
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
