#include "Renderer.h"

#include "../Camera/Camera.h"

#include <iostream>
#include <cmath>

namespace Pelvis
{

static const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main()
{
    gl_Position = uViewProjection * uModel * vec4(aPos, 1.0);
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
        fov * 3.14159265359f / 180.0f;

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
        camera.getYaw() *
        3.14159265359f /
        180.0f;

    const float pitch =
        camera.getPitch() *
        3.14159265359f /
        180.0f;

    const float cy = std::cos(yaw);
    const float sy = std::sin(yaw);

    const float cp = std::cos(pitch);
    const float sp = std::sin(pitch);

    const float fx = cy * cp;
    const float fy = sp;
    const float fz = sy * cp;

    float rx = -sy;
    float ry = 0.0f;
    float rz = cy;

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
                a[0 * 4 + row] * b[column * 4 + 0] +
                a[1 * 4 + row] * b[column * 4 + 1] +
                a[2 * 4 + row] * b[column * 4 + 2] +
                a[3 * 4 + row] * b[column * 4 + 3];
        }
    }

    for (int i = 0; i < 16; ++i)
        result[i] = temp[i];
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
        return false;
    }

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr
            << "GLEW initialization failed.\n";

        return false;
    }

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
        char log[1024];

        glGetShaderInfoLog(
            vertexShader,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr
            << "Vertex shader failed:\n"
            << log
            << '\n';

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
        char log[1024];

        glGetShaderInfoLog(
            fragmentShader,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr
            << "Fragment shader failed:\n"
            << log
            << '\n';

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
        std::cerr
            << "Shader linking failed.\n";

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

    glBindVertexArray(0);

    return true;
}

void Renderer::beginFrame()
{
    if (!m_initialized)
        return;

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

    float model[16] =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

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

    glUseProgram(m_shaderProgram);

    glUniformMatrix4fv(
        m_viewProjectionLocation,
        1,
        GL_FALSE,
        viewProjection
    );

    float vertices[6 * 3 * 41];
    int vertexCount = 0;

    // Grid lines parallel to X.
    for (int z = -20; z <= 20; ++z)
    {
        vertices[vertexCount++] = -20.0f;
        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] = static_cast<float>(z);

        vertices[vertexCount++] = 20.0f;
        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] = static_cast<float>(z);
    }

    // Grid lines parallel to Z.
    for (int x = -20; x <= 20; ++x)
    {
        vertices[vertexCount++] = static_cast<float>(x);
        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] = -20.0f;

        vertices[vertexCount++] = static_cast<float>(x);
        vertices[vertexCount++] = 0.0f;
        vertices[vertexCount++] = 20.0f;
    }

    GLuint gridVAO = 0;
    GLuint gridVBO = 0;

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);

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

    float model[16] =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

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

    glDeleteBuffers(1, &gridVBO);
    glDeleteVertexArrays(1, &gridVAO);
}

void Renderer::drawWorld(
    const Camera& camera
)
{
    drawGrid(camera);
    drawCube(camera);
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
