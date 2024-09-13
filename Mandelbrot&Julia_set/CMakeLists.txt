cmake_minimum_required(VERSION 3.28)
project(ftl_inf)

set(CMAKE_CXX_STANDARD 17)

add_executable(ftl_inf main.cpp
        main.hpp)



set(CMAKE_PREFIX_PATH "${SFML_LIBRARY_DIRECTORY}/lib/cmake/SFML")
set(SFML_DIR "${SFML_LIBRARY_DIRECTORY}/lib/cmake/SFML")


find_package(SFML 2.6.1 COMPONENTS network audio graphics window system REQUIRED)

target_link_libraries(${PROJECT_NAME}
        sfml-network
        sfml-audio
        sfml-graphics
        sfml-window
        sfml-system
)


