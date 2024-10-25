cmake_minimum_required(VERSION 3.5.0)

project(converter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/ffmpeg)

list(APPEND SOURCES
    src/main.cpp
    src/converter.cpp
    src/utils.cpp
)

add_executable(converter ${SOURCES})

target_link_libraries(converter ffmpeg)