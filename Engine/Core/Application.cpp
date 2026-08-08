#include "Application.h"

#include "../Renderer/Renderer.h"

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
        std::cout << "          PELVIS ENGINE 0.1\n";
        std::cout << "====================================\n";

        m_running = true;

        return true;
    }

    int Application::run()
    {
        if (!m_running)
        {
            return 1;
        }

        Renderer renderer;

        if (!renderer.initialize(1280, 720))
        {
            return 1;
        }

        std::cout << "Application initialized successfully.\n";
        std::cout << "Engine loop ready.\n";

        renderer.beginFrame();
        renderer.endFrame();

        renderer.shutdown();

        return 0;
    }

    void Application::shutdown()
    {
        if (!m_running)
        {
            return;
        }

        std::cout << "====================================\n";
        std::cout << "Pelvis Engine shutdown.\n";
        std::cout << "====================================\n";

        m_running = false;
    }
}
