#pragma once

#include <SDL3/SDL.h>
#include <GL/glew.h>

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

    GLuint m_shaderProgram = 0;
    GLuint m_vertexArray = 0;
    GLuint m_vertexBuffer = 0;

    bool m_initialized = false;
};

}