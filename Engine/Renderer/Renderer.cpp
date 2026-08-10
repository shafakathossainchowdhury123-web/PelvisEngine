#include "Renderer.h"

#include <iostream>

namespace Pelvis
{

static const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
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

bool Renderer::initialize(SDL_Window* window)
{
    if (!window)
    {
        std::cerr << "Renderer: invalid SDL window.\n";
        return false;
    }

    m_window = window;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    m_context = SDL_GL_CreateContext(m_window);

    if (!m_context)
    {
        std::cerr
            << "Renderer: OpenGL context creation failed: "
            << SDL_GetError()
            << '\n';

        return false;
    }

    if (!SDL_GL_MakeCurrent(m_window, m_context))
    {
        std::cerr
            << "Renderer: failed to make OpenGL context current: "
            << SDL_GetError()
            << '\n';

        SDL_GL_DestroyContext(m_context);
        m_context = nullptr;

        return false;
    }

    glewExperimental = GL_TRUE;

    GLenum glewResult = glewInit();

    if (glewResult != GLEW_OK)
    {
        std::cerr
            << "Renderer: GLEW initialization failed: "
            << reinterpret_cast<const char*>(
                glewGetErrorString(glewResult)
            )
            << '\n';

        SDL_GL_DestroyContext(m_context);
        m_context = nullptr;

        return false;
    }

    glGetError();

    std::cout << "OpenGL renderer initialized.\n";

    std::cout
        << "OpenGL version: "
        << reinterpret_cast<const char*>(glGetString(GL_VERSION))
        << '\n';

    std::cout
        << "GPU: "
        << reinterpret_cast<const char*>(glGetString(GL_RENDERER))
        << '\n';

    if (!createShaders())
    {
        std::cerr << "Renderer: shader creation failed.\n";
        shutdown();
        return false;
    }

    if (!createTriangleResources())
    {
        std::cerr << "Renderer: triangle resource creation failed.\n";
        shutdown();
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    m_initialized = true;

    return true;
}

bool Renderer::createShaders()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );

    glCompileShader(vertexShader);

    GLint vertexSuccess = GL_FALSE;

    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &vertexSuccess
    );

    if (!vertexSuccess)
    {
        char log[512];

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

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );

    glCompileShader(fragmentShader);

    GLint fragmentSuccess = GL_FALSE;

    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &fragmentSuccess
    );

    if (!fragmentSuccess)
    {
        char log[512];

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

    m_shaderProgram = glCreateProgram();

    glAttachShader(
        m_shaderProgram,
        vertexShader
    );

    glAttachShader(
        m_shaderProgram,
        fragmentShader
    );

    glLinkProgram(m_shaderProgram);

    GLint linkSuccess = GL_FALSE;

    glGetProgramiv(
        m_shaderProgram,
        GL_LINK_STATUS,
        &linkSuccess
    );

    if (!linkSuccess)
    {
        char log[512];

        glGetProgramInfoLog(
            m_shaderProgram,
            sizeof(log),
            nullptr,
            log
        );

        std::cerr
            << "Shader program linking failed:\n"
            << log
            << '\n';

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_shaderProgram);

        m_shaderProgram = 0;

        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Renderer::createTriangleResources()
{
    const float vertices[] =
    {
         0.0f,  0.6f, 0.0f,
        -0.6f, -0.6f, 0.0f,
         0.6f, -0.6f, 0.0f
    };

    glGenVertexArrays(
        1,
        &m_vertexArray
    );

    glGenBuffers(
        1,
        &m_vertexBuffer
    );

    glBindVertexArray(m_vertexArray);

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

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    glBindVertexArray(0);

    return true;
}

void Renderer::beginFrame()
{
    if (!m_initialized)
    {
        return;
    }

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

void Renderer::drawTriangle()
{
    if (!m_initialized)
    {
        return;
    }

    glUseProgram(m_shaderProgram);

    glBindVertexArray(m_vertexArray);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        3
    );

    glBindVertexArray(0);
}

void Renderer::endFrame()
{
    if (!m_initialized)
    {
        return;
    }

    SDL_GL_SwapWindow(m_window);
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
        SDL_GL_DestroyContext(m_context);
        m_context = nullptr;
    }

    m_window = nullptr;
    m_initialized = false;
}

}
