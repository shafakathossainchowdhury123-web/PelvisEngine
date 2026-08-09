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
        void drawTriangle();
        void endFrame();

        void shutdown();

    private:
        bool createTriangleResources();
        bool createShaders();

        SDL_Window* m_window = nullptr;
        SDL_GLContext m_context = nullptr;

        unsigned int m_shaderProgram = 0;
        unsigned int m_vertexArray = 0;
        unsigned int m_vertexBuffer = 0;

        bool m_initialized = false;
    };
}
