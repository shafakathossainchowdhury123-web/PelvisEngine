#pragma once

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
    };
}
