cmake_minimum_required(VERSION 3.20)

project(PelvisEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

add_executable(PelvisEngine
    Engine/main.cpp
)

target_link_libraries(PelvisEngine
    PRIVATE SDL3::SDL3
)

target_compile_options(PelvisEngine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
