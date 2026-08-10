cmake_minimum_required(VERSION 3.20)

project(PelvisEngine VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# ============================================================
# SDL3
# ============================================================

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

# ============================================================
# OpenGL 
# ============================================================

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
# ============================================================ 
add_executable(PelvisEngine
    Engine/main.cpp
    Engine/Core/Application.cpp
    Engine/Renderer/Renderer.cpp
    Engine/Renderer/Mesh.cpp
    Engine/Graphics/Model.cpp
    Engine/Camera/Camera.cpp
    Engine/Player/PlayerController.cpp
    Engine/Player/Player.cpp
    Engine/World/Transform.cpp
    Engine/World/Entity.cpp
    Engine/World/World.cpp
    Engine/Gameplay/ActionSystem.cpp
    Engine/Gameplay/ActionMap.cpp
    Engine/Gameplay/MeleeWeapon.cpp
    Engine/Gameplay/Firearm.cpp
    Engine/Gameplay/Inventory.cpp
    Engine/Input/Input.cpp
    Engine/Input/ActionMap.cpp
) 
# ============================================================
target_include_directories(PelvisEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine)


 target_link_libraries(PelvisEngine PRIVATE
    SDL3::SDL3
    OpenGL::GL
    GLEW::GLEW
)
# ============================================================
# Warnings
# ============================================================

if(MSVC)

    target_compile_options(PelvisEngine PRIVATE
        /W4
    )

else()

    target_compile_options(PelvisEngine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()



find_package(assimp REQUIRED)

target_link_libraries(PelvisEngine PRIVATE assimp::assimp)
