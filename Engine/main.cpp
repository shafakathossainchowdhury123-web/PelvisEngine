#include <SDL3/SDL.h>

#include <iostream>

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL initialization failed: "
                  << SDL_GetError() << '\n';

        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Pelvis Engine 0.1",
        1280,
        720,
        SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        std::cerr << "Window creation failed: "
                  << SDL_GetError() << '\n';

        SDL_Quit();
        return 1;
    }

    std::cout << "====================================\n";
    std::cout << "        PELVIS ENGINE 0.1\n";
    std::cout << "====================================\n";
    std::cout << "SDL initialized successfully.\n";
    std::cout << "Window created successfully.\n";
    std::cout << "====================================\n";

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        }

        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
