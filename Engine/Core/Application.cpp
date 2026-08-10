#include "Application.h"

#include "../Input/Input.h"

#include <SDL3/SDL.h>

#include <iostream>
#include <chrono>

namespace Pelvis
{

Application::Application() = default;

Application::~Application()
{
    shutdown();
}


// ============================================================
// INITIALIZE
// ============================================================

bool Application::initialize()
{
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "          PELVIS ENGINE 0.5\n";
    std::cout << "====================================\n";

    Input::initialize();

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        std::cout
            << "SDL video unavailable: "
            << SDL_GetError()
            << '\n';

        std::cout
            << "Running Pelvis Engine in HEADLESS mode.\n";

        m_running = true;
        return true;
    }

    m_window = SDL_CreateWindow(
        "Pelvis Engine 0.5",
        1280,
        720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!m_window)
    {
        std::cout
            << "Window unavailable: "
            << SDL_GetError()
            << '\n';

        std::cout
            << "Running Pelvis Engine in HEADLESS mode.\n";

        SDL_Quit();

        m_running = true;
        return true;
    }

    if (!m_renderer.initialize(m_window))
    {
        std::cerr
            << "Renderer initialization failed.\n";

        SDL_DestroyWindow(m_window);
        m_window = nullptr;

        SDL_Quit();

        m_running = true;
        return true;
    }

    m_running = true;

    std::cout
        << "Pelvis Engine initialized successfully.\n";

    std::cout
        << "Graphics mode: WINDOWED\n";

    return true;
}


// ============================================================
// INPUT PROCESSING
// ============================================================

void Application::processInput(float deltaTime)
{
    float forward = 0.0f;
    float right = 0.0f;
    float up = 0.0f;


    // --------------------------------------------------------
    // MOVEMENT
    // --------------------------------------------------------

    if (Input::isKeyDown(Key::W))
        forward += 1.0f;

    if (Input::isKeyDown(Key::S))
        forward -= 1.0f;

    if (Input::isKeyDown(Key::D))
        right += 1.0f;

    if (Input::isKeyDown(Key::A))
        right -= 1.0f;


    // --------------------------------------------------------
    // VERTICAL MOVEMENT
    // --------------------------------------------------------

    if (Input::isKeyDown(Key::Space))
        up += 1.0f;

    if (Input::isKeyDown(Key::LeftControl))
        up -= 1.0f;


    // --------------------------------------------------------
    // SPRINT
    // --------------------------------------------------------

    if (Input::isKeyDown(Key::LeftShift))
    {
        forward *= 2.0f;
        right *= 2.0f;
        up *= 2.0f;
    }


    // --------------------------------------------------------
    // CAMERA MOVEMENT
    // --------------------------------------------------------

    m_camera.move(
        forward,
        right,
        up,
        deltaTime
    );


    // --------------------------------------------------------
    // MOUSE CAMERA
    // --------------------------------------------------------

    constexpr float mouseSensitivity = 0.10f;

    const float mouseX =
        Input::getMouseDeltaX() * mouseSensitivity;

    const float mouseY =
        Input::getMouseDeltaY() * mouseSensitivity;

    m_camera.rotate(
        mouseX,
        -mouseY
    );


    // --------------------------------------------------------
    // ESCAPE
    // --------------------------------------------------------

    if (Input::isKeyPressed(Key::Escape))
    {
        m_running = false;
    }


    // --------------------------------------------------------
    // PRIMARY ACTION
    // LEFT CLICK
    // --------------------------------------------------------

    if (Input::isMousePressed(MouseButton::Left))
    {
        std::cout
            << "[Input] Primary Action\n";
    }


    // --------------------------------------------------------
    // SECONDARY ACTION
    // RIGHT CLICK
    // --------------------------------------------------------

    if (Input::isMousePressed(MouseButton::Right))
    {
        std::cout
            << "[Input] Secondary Action\n";
    }


    // --------------------------------------------------------
    // INTERACTION
    // E
    // --------------------------------------------------------

    if (Input::isKeyPressed(Key::E))
    {
        std::cout
            << "[Input] Interact\n";
    }


    // --------------------------------------------------------
    // RELOAD
    // R
    // --------------------------------------------------------

    if (Input::isKeyPressed(Key::R))
    {
        std::cout
            << "[Input] Reload\n";
    }


    // --------------------------------------------------------
    // WEAPON / ITEM SLOTS
    // --------------------------------------------------------

    if (Input::isKeyPressed(Key::Num1))
        std::cout << "[Input] Slot 1\n";

    if (Input::isKeyPressed(Key::Num2))
        std::cout << "[Input] Slot 2\n";

    if (Input::isKeyPressed(Key::Num3))
        std::cout << "[Input] Slot 3\n";

    if (Input::isKeyPressed(Key::Num4))
        std::cout << "[Input] Slot 4\n";

    if (Input::isKeyPressed(Key::Num5))
        std::cout << "[Input] Slot 5\n";

    if (Input::isKeyPressed(Key::Num6))
        std::cout << "[Input] Slot 6\n";

    if (Input::isKeyPressed(Key::Num7))
        std::cout << "[Input] Slot 7\n";

    if (Input::isKeyPressed(Key::Num8))
        std::cout << "[Input] Slot 8\n";

    if (Input::isKeyPressed(Key::Num9))
        std::cout << "[Input] Slot 9\n";
}


// ============================================================
// MAIN LOOP
// ============================================================

int Application::run()
{
    std::cout
        << "Entering main loop...\n";


    // --------------------------------------------------------
    // HEADLESS MODE
    // --------------------------------------------------------

    if (!m_window)
    {
        std::cout
            << "Graphics device unavailable.\n";

        std::cout
            << "Pelvis Engine core is running in headless mode.\n";

        for (int i = 0; i < 10 && m_running; ++i)
        {
            SDL_Delay(100);
        }

        m_running = false;

        return 0;
    }


    // --------------------------------------------------------
    // DELTA TIME
    // --------------------------------------------------------

    auto previousTime =
        std::chrono::steady_clock::now();


    // --------------------------------------------------------
    // GAME LOOP
    // --------------------------------------------------------

    while (m_running)
    {
        auto currentTime =
            std::chrono::steady_clock::now();

        const float deltaTime =
            std::chrono::duration<float>(
                currentTime - previousTime
            ).count();

        previousTime = currentTime;


        // ----------------------------------------------------
        // EVENTS
        // ----------------------------------------------------

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            Input::processEvent(event);

            if (event.type == SDL_EVENT_QUIT)
            {
                m_running = false;
            }
        }


        // ----------------------------------------------------
        // INPUT
        // ----------------------------------------------------

        processInput(deltaTime);


        // ----------------------------------------------------
        // RENDER
        // ----------------------------------------------------

        m_renderer.beginFrame();

        m_renderer.drawTriangle();

        m_renderer.endFrame();


        // ----------------------------------------------------
        // INPUT FRAME UPDATE
        // ----------------------------------------------------

        Input::update();


        // ----------------------------------------------------
        // SMALL CPU YIELD
        // ----------------------------------------------------

        SDL_Delay(1);
    }

    return 0;
}


// ============================================================
// SHUTDOWN
// ============================================================

void Application::shutdown()
{
    if (m_window)
    {
        m_renderer.shutdown();

        SDL_DestroyWindow(m_window);

        m_window = nullptr;

        SDL_Quit();
    }

    m_running = false;

    std::cout
        << "====================================\n";

    std::cout
        << "Pelvis Engine shutdown.\n";

    std::cout
        << "====================================\n";
}

} // namespace Pelvis
