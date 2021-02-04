cmake_minimum_required(VERSION 3.0.0)
project(Shooty VERSION 1.0.0)

include(CTest)
enable_testing()

find_package(SFML 2.5
    COMPONENTS graphics audio
    REQUIRED
)

add_executable(Shooty 
    main.cpp)

target_include_directories(Shooty
    PRIVATE /usr/local/include
)

target_link_libraries(Shooty
    sfml-graphics
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
