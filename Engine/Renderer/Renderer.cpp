#include "Renderer.h"

#include <iostream>

namespace Pelvis
{
    bool Renderer::initialize(int width, int height)
    {
        if (width <= 0 || height <= 0)
        {
            std::cerr << "Renderer: invalid resolution.\n";
            return false;
        }

        m_width = width;
        m_height = height;
        m_initialized = true;

        std::cout
            << "Renderer initialized: "
            << m_width << "x"
            << m_height << '\n';

        return true;
    }

    void Renderer::beginFrame()
    {
        if (!m_initialized)
        {
            return;
        }

        // Rendering commands will be added here.
    }

    void Renderer::endFrame()
    {
        if (!m_initialized)
        {
            return;
        }

        // Frame presentation will be added here.
    }

    void Renderer::shutdown()
    {
        if (!m_initialized)
        {
            return;
        }

        std::cout << "Renderer shutdown.\n";

        m_initialized = false;
        m_width = 0;
        m_height = 0;
    }

    int Renderer::width() const
    {
        return m_width;
    }

    int Renderer::height() const
    {
        return m_height;
    }
}
