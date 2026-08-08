#pragma once

#include <SDL3/SDL.h>

#include "../Renderer/Renderer.h"

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
        bool m_running = false;

        SDL_Window* m_window = nullptr;

        Renderer m_renderer;
    };
}
