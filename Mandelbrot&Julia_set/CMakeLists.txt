cmake_minimum_required(VERSION 3.16)
project(ftl_inf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Путь к SFML 2.6.2
set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")
find_package(SFML 2 REQUIRED COMPONENTS system window graphics)

add_executable(ftl_inf main.cpp)

target_link_libraries(ftl_inf PRIVATE sfml-system sfml-window sfml-graphics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

# macOS фреймворки — обязательны для корректной линковки
if(APPLE)
    target_link_libraries(ftl_inf PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
    )
endif()