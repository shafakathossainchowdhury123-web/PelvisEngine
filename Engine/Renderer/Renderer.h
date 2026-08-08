#pragma once

#include <cstdint>

namespace Pelvis
{
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool initialize(int width, int height);
        void beginFrame();
        void endFrame();
        void shutdown();

        int width() const;
        int height() const;

    private:
        int m_width = 0;
        int m_height = 0;
        bool m_initialized = false;
    };
}
