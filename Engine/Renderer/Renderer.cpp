#include "Renderer.h"

#include <SDL3/SDL_opengl.h>

#include <iostream>

namespace Pelvis
{
    bool Renderer::initialize(SDL_Window* window)
    {
        if (!window)
        {
            std::cerr << "Renderer: invalid window.\n";
            return false;
        }

        m_window = window;

        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_MAJOR_VERSION,
            3
        );

        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_MINOR_VERSION,
            3
        );

        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE
        );

        m_context = SDL_GL_CreateContext(m_window);

        if (!m_context)
        {
            std::cerr
                << "OpenGL context creation failed: "
                << SDL_GetError()
                << '\n';

            return false;
        }

        if (!SDL_GL_MakeCurrent(m_window, m_context))
        {
            std::cerr
                << "Failed to make OpenGL context current: "
                << SDL_GetError()
                << '\n';

            SDL_GL_DestroyContext(m_context);
            m_context = nullptr;

            return false;
        }

        SDL_GL_SetSwapInterval(1);

        std::cout << "OpenGL renderer initialized.\n";

        const GLubyte* version = glGetString(GL_VERSION);

        if (version)
        {
            std::cout
                << "OpenGL version: "
                << version
                << '\n';
        }

        m_initialized = true;

        return true;
    }

    void Renderer::beginFrame()
    {
        if (!m_initialized)
        {
            return;
        }

        glClearColor(
            0.08f,
            0.10f,
            0.14f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::endFrame()
    {
        if (!m_initialized)
        {
            return;
        }

        SDL_GL_SwapWindow(m_window);
    }

    void Renderer::shutdown()
    {
        if (m_context)
        {
            SDL_GL_DestroyContext(m_context);
            m_context = nullptr;
        }

        m_window = nullptr;
        m_initialized = false;

        std::cout << "OpenGL renderer shutdown.\n";
    }
}
