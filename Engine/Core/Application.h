#pragma once

#include <SDL3/SDL.h>

#include "../Renderer/Renderer.h"
#include "../Camera/Camera.h"
#include "../Player/PlayerController.h"

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
        Camera m_camera;
        PlayerController m_playerController;
    };
}

