#pragma once

#include <SDL3/SDL.h>

namespace Pelvis
{
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool initialize(SDL_Window* window);
        void beginFrame();
        void endFrame();
        void shutdown();

    private:
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_context = nullptr;
        bool m_initialized = false;
    };
}
