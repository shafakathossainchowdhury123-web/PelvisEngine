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

    Renderer();
    ~Renderer();

    bool initialize(SDL_Window* window);

    void beginFrame();

    void drawTriangle();

    void drawCube(
        const Camera& camera
    );

    void drawCube(
        const Camera& camera,
        const Entity& entity
    );

    void drawModel(
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

    bool isInitialized() const;

private:

    bool createShaders();

    bool createCubeResources();

    SDL_Window* m_window;
    SDL_GLContext m_context;

    GLuint m_shaderProgram;

    GLuint m_vertexArray;
    GLuint m_vertexBuffer;

    GLint m_modelLocation;
    GLint m_viewProjectionLocation;

    bool m_initialized;
};

}
