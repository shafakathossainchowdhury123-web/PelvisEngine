#include "Application.h"

#include "../Renderer/Renderer.h"

#include <SDL3/SDL.h>

#include <iostream>

namespace Pelvis
{
    Application::Application() = default;

    Application::~Application()
    {
        shutdown();
    }

    bool Application::initialize()
    {
        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << "          PELVIS ENGINE 0.2\n";
        std::cout << "====================================\n";

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            std::cerr
                << "SDL initialization failed: "
                << SDL_GetError()
                << '\n';

            return false;
        }

        m_window = SDL_CreateWindow(
            "Pelvis Engine 0.2",
            1280,
            720,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );

        if (!m_window)
        {
            std::cerr
                << "Window creation failed: "
                << SDL_GetError()
                << '\n';

            SDL_Quit();

            return false;
        }

        if (!m_renderer.initialize(m_window))
        {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;

            SDL_Quit();

            return false;
        }

        m_running = true;

        return true;
    }

    int Application::run()
    {
        while (m_running)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    m_running = false;
                }

                if (event.type == SDL_EVENT_KEY_DOWN)
                {
                    if (event.key.key == SDLK_ESCAPE)
                    {
                        m_running = false;
                    }
                }
            }

            m_renderer.beginFrame();

m_renderer.drawTriangle();

m_renderer.endFrame();

            SDL_Delay(1);
        }

        return 0;
    }

    void Application::shutdown()
    {
        if (!m_running && !m_window)
        {
            return;
        }

        m_renderer.shutdown();

        if (m_window)
        {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }

        SDL_Quit();

        m_running = false;

        std::cout << "====================================\n";
        std::cout << "Pelvis Engine shutdown.\n";
        std::cout << "====================================\n";
    }
}
