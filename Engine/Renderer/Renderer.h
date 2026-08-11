#pragma once

#include <SDL3/SDL.h>
#include <GL/glew.h>

namespace Pelvis
{

class Camera;
class Entity;
class World;

class Renderer
{
public:

    Renderer() = default;
    ~Renderer() = default;

    bool initialize(SDL_Window* window);

    void beginFrame();

    void drawCube(
        const Camera& camera
    );

    void drawCube(
        const Camera& camera,
        const Entity& entity
    );

    void drawGrid(
        const Camera& camera
    );

    void drawWorld(
        const Camera& camera
    );

    void drawWorld(
        const Camera& camera,
        const World& world
    );

    void endFrame();

    void shutdown();

private:

    bool createShaders();

    bool createCubeResources();

    SDL_Window* m_window = nullptr;

    SDL_GLContext m_context = nullptr;

    GLuint m_shaderProgram = 0;

    GLuint m_vertexArray = 0;

    GLuint m_vertexBuffer = 0;

    GLint m_viewProjectionLocation = -1;

    GLint m_modelLocation = -1;

    bool m_initialized = false;
};

}
