#pragma once

#include <SDL3/SDL.h>

#include "../Renderer/Renderer.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"

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

    Player m_player;
        void processInput(float deltaTime);

        bool m_running = false;

        SDL_Window* m_window = nullptr;

        Renderer m_renderer;
    };
}

