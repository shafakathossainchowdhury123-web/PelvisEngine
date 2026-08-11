#include "Renderer.h"

#include "../Camera/Camera.h"
#include "../Graphics/Model.h"
#include "../World/Entity.h"
#include "../World/World.h"
#include "../World/Transform.h"

#include <iostream>
#include <cmath>

namespace Pelvis
{

Renderer::Renderer()
    : m_window(nullptr)
    , m_context(nullptr)
    , m_shaderProgram(0)
    , m_vertexArray(0)
    , m_vertexBuffer(0)
    , m_modelLocation(-1)
    , m_viewProjectionLocation(-1)
    , m_initialized(false)
{
}

Renderer::~Renderer()
{
    shutdown();
}


static constexpr float PI = 3.14159265359f;

static const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main()
{
    gl_Position =
        uViewProjection *
        uModel *
        vec4(aPos, 1.0);
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.15, 0.65, 1.0, 1.0);
}
)";

static void makeIdentity(float* m)
{
    for (int i = 0; i < 16; ++i)
        m[i] = 0.0f;

    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
}

static void makePerspective(
    float* m,
    float fov,
    float aspect,
    float nearPlane,
    float farPlane
)
{
    for (int i = 0; i < 16; ++i)
        m[i] = 0.0f;

    const float radians =
        fov * PI / 180.0f;

    const float f =
        1.0f / std::tan(radians * 0.5f);

    m[0] = f / aspect;
    m[5] = f;

    m[10] =
        (farPlane + nearPlane) /
        (nearPlane - farPlane);

    m[11] = -1.0f;

    m[14] =
        (2.0f * farPlane * nearPlane) /
        (nearPlane - farPlane);
}

static void makeView(
    float* m,
    const Camera& camera
)
{
    const float yaw =
        camera.getYaw() * PI / 180.0f;

    const float pitch =
        camera.getPitch() * PI / 180.0f;

    const float cy = std::cos(yaw);
    const float sy = std::sin(yaw);

    const float cp = std::cos(pitch);
    const float sp = std::sin(pitch);

    // Forward vector.
    const float fx = cy * cp;
    const float fy = sp;
    const float fz = sy * cp;

    // Right vector.
    const float rx = -sy;
    const float ry = 0.0f;
    const float rz = cy;

    // Up = right x forward.
    const float ux =
        ry * fz - rz * fy;

    const float uy =
        rz * fx - rx * fz;

    const float uz =
        rx * fy - ry * fx;

    m[0] = rx;
    m[1] = ux;
    m[2] = -fx;
    m[3] = 0.0f;

    m[4] = ry;
    m[5] = uy;
    m[6] = -fy;
    m[7] = 0.0f;

    m[8] = rz;
    m[9] = uz;
    m[10] = -fz;
    m[11] = 0.0f;

    m[12] =
        -(rx * camera.getX() +
          ry * camera.getY() +
          rz * camera.getZ());

    m[13] =
        -(ux * camera.getX() +
          uy * camera.getY() +
          uz * camera.getZ());

    m[14] =
        fx * camera.getX() +
        fy * camera.getY() +
        fz * camera.getZ();

    m[15] = 1.0f;
}

static void multiplyMatrix(
    float* result,
    const float* a,
    const float* b
)
{
    float temp[16];

    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            temp[column * 4 + row] =
                a[0 * 4 + row] *
                    b[column * 4 + 0] +
                a[1 * 4 + row] *
                    b[column * 4 + 1] +
                a[2 * 4 + row] *
                    b[column * 4 + 2] +
                a[3 * 4 + row] *
                    b[column * 4 + 3];
        }
    }

    for (int i = 0; i < 16; ++i)
        result[i] = temp[i];
}

static void makeModelMatrix(
    float* m,
    const Transform& transform
)
{
    const float rx =
        transform.getRotationX() * PI / 180.0f;

    const float ry =
        transform.getRotationY() * PI / 180.0f;

    const float rz =
        transform.getRotationZ() * PI / 180.0f;

    const float cx = std::cos(rx);
    const float sx = std::sin(rx);

    const float cy = std::cos(ry);
    const float sy = std::sin(ry);

    const float cz = std::cos(rz);
    const float sz = std::sin(rz);

    const float scaleX = transform.getScaleX();
    const float scaleY = transform.getScaleY();
    const float scaleZ = transform.getScaleZ();

    // Rz * Ry * Rx
    m[0] =
        cz * cy * scaleX;

    m[1] =
        sz * cy * scaleX;

    m[2] =
        -sy * scaleX;

    m[3] = 0.0f;

    m[4] =
        (cz * sy * sx - sz * cx) *
        scaleY;

    m[5] =
        (sz * sy * sx + cz * cx) *
        scaleY;

    m[6] =
        cy * sx *
        scaleY;

    m[7] = 0.0f;

    m[8] =
        (cz * sy * cx + sz * sx) *
        scaleZ;

    m[9] =
        (sz * sy * cx - cz * sx) *
        scaleZ;

    m[10] =
        cy * cx *
        scaleZ;

    m[11] = 0.0f;

    m[12] = transform.getPositionX();
    m[13] = transform.getPositionY();
    m[14] = transform.getPositionZ();
    m[15] = 1.0f;
}

bool Renderer::initialize(SDL_Window* window)
{
    if (!window)
        return false;

    m_window = window;

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION,
        3
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MINOR_VERSION,
        3
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    m_context =
        SDL_GL_CreateContext(m_window);

    if (!m_context)
    {
        std::cerr
            << "OpenGL context creation failed: "
            << SDL_GetError()
            << '\n';

        return false;
    }

    if (!SDL_GL_MakeCurrent(
        m_window,
        m_context))
    {
        std::cerr
            << "Failed to make OpenGL context current: "
            << SDL_GetError()
            << '\n';

        return false;
    }

    glewExperimental = GL_TRUE;

    const GLenum glewResult = glewInit();

    if (glewResult != GLEW_OK)
    {
        std::cerr
            << "GLEW initialization failed: "
            << reinterpret_cast<const char*>(
                glewGetErrorString(glewResult))
            << '\n';

        return false;
    }

    // GLEW can generate a harmless GL_INVALID_ENUM
    // while initializing a core profile context.
    glGetError();

    std::cout
        << "OpenGL version: "
        << reinterpret_cast<const char*>(
            glGetString(GL_VERSION))
        << '\n';

    std::cout
        << "GPU: "
        << reinterpret_cast<const char*>(
            glGetString(GL_RENDERER))
        << '\n';

    if (!createShaders())
        return false;

    if (!createCubeResources())
        return false;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    SDL_GL_SetSwapInterval(1);

    m_initialized = true;

    std::cout
        << "3D renderer initialized.\n";

    return true;
}

bool Renderer::createShaders()
{
    GLuint vertexShader =
        glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );

    glCompileShader(vertexShader);

    GLint success = GL_FALSE;

    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        char log[1024] = {};

        glGetShaderInfoLog(
            vertexShader,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr
            << "Vertex shader compilation failed:\n"
            << log
            << '\n';

        glDeleteShader(vertexShader);

        return false;
    }

    GLuint fragmentShader =
        glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );

    glCompileShader(fragmentShader);

    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        char log[1024] = {};

        glGetShaderInfoLog(
            fragmentShader,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr
            << "Fragment shader compilation failed:\n"
            << log
            << '\n';

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return false;
    }

    m_shaderProgram =
        glCreateProgram();

    glAttachShader(
        m_shaderProgram,
        vertexShader
    );

    glAttachShader(
        m_shaderProgram,
        fragmentShader
    );

    glLinkProgram(
        m_shaderProgram
    );

    glGetProgramiv(
        m_shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success)
    {
        char log[1024] = {};

        glGetProgramInfoLog(
            m_shaderProgram,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr
            << "Shader linking failed:\n"
            << log
            << '\n';

        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;

        return false;
    }

    m_viewProjectionLocation =
        glGetUniformLocation(
            m_shaderProgram,
            "uViewProjection"
        );

    m_modelLocation =
        glGetUniformLocation(
            m_shaderProgram,
            "uModel"
        );

    return true;
}

bool Renderer::createCubeResources()
{
    const float vertices[] =
    {
        // Front
        -1,-1, 1,
         1,-1, 1,
         1, 1, 1,

        -1,-1, 1,
         1, 1, 1,
        -1, 1, 1,

        // Back
         1,-1,-1,
        -1,-1,-1,
        -1, 1,-1,

         1,-1,-1,
        -1, 1,-1,
         1, 1,-1,

        // Left
        -1,-1,-1,
        -1,-1, 1,
        -1, 1, 1,

        -1,-1,-1,
        -1, 1, 1,
        -1, 1,-1,

        // Right
         1,-1, 1,
         1,-1,-1,
         1, 1,-1,

         1,-1, 1,
         1, 1,-1,
         1, 1, 1,

        // Top
        -1, 1, 1,
         1, 1, 1,
         1, 1,-1,

        -1, 1, 1,
         1, 1,-1,
        -1, 1,-1,

        // Bottom
        -1,-1,-1,
         1,-1,-1,
         1,-1, 1,

        -1,-1,-1,
         1,-1, 1,
        -1,-1, 1
    };

    glGenVertexArrays(
        1,
        &m_vertexArray
    );

    glGenBuffers(
        1,
        &m_vertexBuffer
    );

    glBindVertexArray(
        m_vertexArray
    );

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_vertexBuffer
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void Renderer::beginFrame()
{
    if (!m_initialized)
        return;

    glViewport(
        0,
        0,
        1280,
        720
    );

    int width = 1280;
    int height = 720;

    SDL_GetWindowSize(
        m_window,
        &width,
        &height
    );

    if (width <= 0)
        width = 1280;

    if (height <= 0)
        height = 720;

    glViewport(
        0,
        0,
        width,
        height
    );

    glClearColor(
        0.05f,
        0.07f,
        0.10f,
        1.0f
    );

    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
}

void Renderer::drawCube(
    const Camera& camera
)
{
    if (!m_initialized)
        return;

    int width = 1280;
    int height = 720;

    SDL_GetWindowSize(
        m_window,
        &width,
        &height
    );

    if (height <= 0)
        height = 1;

    float projection[16];
    float view[16];
    float viewProjection[16];
    float model[16];

    makePerspective(
        projection,
        camera.getFOV(),
        static_cast<float>(width) /
            static_cast<float>(height),
        camera.getNearPlane(),
        camera.getFarPlane()
    );

    makeView(
        view,
        camera
    );

    multiplyMatrix(
        viewProjection,
        projection,
        view
    );

    makeIdentity(model);

    glUseProgram(
        m_shaderProgram
    );

    glUniformMatrix4fv(
        m_viewProjectionLocation,
        1,
        GL_FALSE,
        viewProjection
    );

    glUniformMatrix4fv(
        m_modelLocation,
        1,
        GL_FALSE,
        model
    );

    glBindVertexArray(
        m_vertexArray
    );

    glDrawArrays(
        GL_TRIANGLES,
        0,
        36
    );

    glBindVertexArray(0);
}

void Renderer::drawCube(
    const Camera& camera,
    const Entity& entity
)
{
    if (!m_initialized)
        return;

    int width = 1280;
    int height = 720;

    SDL_GetWindowSize(
        m_window,
        &width,
        &height
    );

    if (height <= 0)
        height = 1;

    float projection[16];
    float view[16];
    float viewProjection[16];
    float model[16];

    makePerspective(
        projection,
        camera.getFOV(),
        static_cast<float>(width) /
            static_cast<float>(height),
        camera.getNearPlane(),
        camera.getFarPlane()
    );

    makeView(
        view,
        camera
    );

    multiplyMatrix(
        viewProjection,
        projection,
        view
    );

    makeModelMatrix(
        model,
        entity.getTransform()
    );

    glUseProgram(
        m_shaderProgram
    );

    glUniformMatrix4fv(
        m_viewProjectionLocation,
        1,
        GL_FALSE,
        viewProjection
    );

    glUniformMatrix4fv(
        m_modelLocation,
        1,
        GL_FALSE,
        model
    );

    glBindVertexArray(
        m_vertexArray
    );

    glDrawArrays(
        GL_TRIANGLES,
        0,
        36
    );

    glBindVertexArray(0);
}

void Renderer::drawGrid(
    const Camera& camera
)
{
    if (!m_initialized)
        return;

    int width = 1280;
    int height = 720;

    SDL_GetWindowSize(
        m_window,
        &width,
        &height
    );

    if (height <= 0)
        height = 1;

    float projection[16];
    float view[16];
    float viewProjection[16];

    makePerspective(
        projection,
        camera.getFOV(),
        static_cast<float>(width) /
            static_cast<float>(height),
        camera.getNearPlane(),
        camera.getFarPlane()
    );

    makeView(
        view,
        camera
    );

    multiplyMatrix(
        viewProjection,
        projection,
        view
    );

    glUseProgram(
        m_shaderProgram
    );

    glUniformMatrix4fv(
        m_viewProjectionLocation,
        1,
        GL_FALSE,
        viewProjection
    );

    float vertices[492];
    int vertexCount = 0;

    for (int z = -20; z <= 20; ++z)
    {
        vertices[vertexCount++] = -20.0f;
        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] =
            static_cast<float>(z);

        vertices[vertexCount++] = 20.0f;
        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] =
            static_cast<float>(z);
    }

    for (int x = -20; x <= 20; ++x)
    {
        vertices[vertexCount++] =
            static_cast<float>(x);

        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] = -20.0f;

        vertices[vertexCount++] =
            static_cast<float>(x);

        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] = 20.0f;
    }

    GLuint gridVAO = 0;
    GLuint gridVBO = 0;

    glGenVertexArrays(
        1,
        &gridVAO
    );

    glGenBuffers(
        1,
        &gridVBO
    );

    glBindVertexArray(
        gridVAO
    );

    glBindBuffer(
        GL_ARRAY_BUFFER,
        gridVBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        vertexCount * sizeof(float),
        vertices,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

    glEnableVertexAttribArray(0);

    float model[16];

    makeIdentity(model);

    glUniformMatrix4fv(
        m_modelLocation,
        1,
        GL_FALSE,
        model
    );

    glLineWidth(1.0f);

    glDrawArrays(
        GL_LINES,
        0,
        vertexCount / 3
    );

    glBindVertexArray(0);

    glDeleteBuffers(
        1,
        &gridVBO
    );

    glDeleteVertexArrays(
        1,
        &gridVAO
    );
}

void Renderer::drawModel(
    const Camera& camera,
    const Entity& entity
)
{
    if (!m_initialized)
        return;

    const Model* model =
        entity.getModel();

    if (!model ||
        !model->isLoaded())
    {
        return;
    }

    int width = 1280;
    int height = 720;

    SDL_GetWindowSize(
        m_window,
        &width,
        &height
    );

    if (height <= 0)
        height = 1;

    float projection[16];
    float view[16];
    float viewProjection[16];
    float modelMatrix[16];

    makePerspective(
        projection,
        camera.getFOV(),
        static_cast<float>(width) /
            static_cast<float>(height),
        camera.getNearPlane(),
        camera.getFarPlane()
    );

    makeView(
        view,
        camera
    );

    multiplyMatrix(
        viewProjection,
        projection,
        view
    );

    makeModelMatrix(
        modelMatrix,
        entity.getTransform()
    );

    glUseProgram(
        m_shaderProgram
    );

    glUniformMatrix4fv(
        m_viewProjectionLocation,
        1,
        GL_FALSE,
        viewProjection
    );

    glUniformMatrix4fv(
        m_modelLocation,
        1,
        GL_FALSE,
        modelMatrix
    );

    model->draw();
}

void Renderer::drawWorld(
    const Camera& camera
)
{
    drawGrid(camera);
    drawCube(camera);
}

void Renderer::drawWorld(
    const Camera& camera,
    const World& world
)
{
    drawGrid(camera);

    for (const auto& entity : world.getEntities())
    {
        if (!entity)
            continue;

        if (entity->getModel() &&
            entity->getModel()->isLoaded())
        {
            drawModel(
                camera,
                *entity
            );
        }
        else
        {
            drawCube(
                camera,
                *entity
            );
        }
    }
}

void Renderer::endFrame()
{
    if (!m_initialized)
        return;

    SDL_GL_SwapWindow(
        m_window
    );
}

void Renderer::shutdown()
{
    if (m_vertexBuffer)
    {
        glDeleteBuffers(
            1,
            &m_vertexBuffer
        );

        m_vertexBuffer = 0;
    }

    if (m_vertexArray)
    {
        glDeleteVertexArrays(
            1,
            &m_vertexArray
        );

        m_vertexArray = 0;
    }

    if (m_shaderProgram)
    {
        glDeleteProgram(
            m_shaderProgram
        );

        m_shaderProgram = 0;
    }

    if (m_context)
    {
        SDL_GL_DestroyContext(
            m_context
        );

        m_context = nullptr;
    }

    m_window = nullptr;
    m_initialized = false;
}

}
