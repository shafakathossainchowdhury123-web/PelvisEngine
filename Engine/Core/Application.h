#pragma once

#include <SDL3/SDL.h>

#include "../Assets/AssetManager.h"
#include "../Renderer/Renderer.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"
#include "../World/World.h"

namespace Pelvis
{

class Application
{
public:

    Application();
    ~Application();

    bool initialize();
    int run();
    void shutdown();

private:

    void processInput(float deltaTime);

    bool m_running = false;

    SDL_Window* m_window = nullptr;

    Renderer m_renderer;

    AssetManager m_assets;

    Player m_player;

    World m_world;

    Entity* m_cube1 = nullptr;
    Entity* m_cube2 = nullptr;
    Entity* m_cube3 = nullptr;
};

}
