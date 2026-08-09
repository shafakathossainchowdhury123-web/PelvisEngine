#include "Renderer.h"

#include <glad/gl.h>

#include <iostream>

namespace Pelvis
{
    namespace
    {
        const char* vertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec3 aPosition;

            void main()
            {
                gl_Position = vec4(aPosition, 1.0);
            }
        )";

        const char* fragmentShaderSource = R"(
            #version 330 core

            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(0.15, 0.65, 1.0, 1.0);
            }
        )";

        bool checkShader(unsigned int shader, const char* name)
        {
            int success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                char infoLog[512]{};

                glGetShaderInfoLog(
                    shader,
                    sizeof(infoLog),
                    nullptr,
                    infoLog
                );

                std::cerr
                    << "Shader compilation failed ("
                    << name
                    << "):\n"
                    << infoLog
                    << '\n';

                return false;
            }

            return true;
        }

        bool checkProgram(unsigned int program)
        {
            int success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &success);

            if (!success)
            {
                char infoLog[512]{};

                glGetProgramInfoLog(
                    program,
                    sizeof(infoLog),
                    nullptr,
                    infoLog
                );

                std::cerr
                    << "Shader program linking failed:\n"
                    << infoLog
                    << '\n';

                return false;
            }

            return true;
        }
    }

    bool Renderer::initialize(SDL_Window* window)
    {
        if (!window)
        {
            std::cerr << "Renderer: invalid window.\n";
            return false;
        }

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

        m_context = SDL_GL_CreateContext(m_window);

        if (!m_context)
        {
            std::cerr
                << "OpenGL context creation failed: "
                << SDL_GetError()
                << '\n';

            return false;
        }

        if (!SDL_GL_MakeCurrent(m_window, m_context))
        {
            std::cerr
                << "Failed to make OpenGL context current: "
                << SDL_GetError()
                << '\n';

            SDL_GL_DestroyContext(m_context);
            m_context = nullptr;

            return false;
        }

        if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
        {
            std::cerr
                << "Failed to initialize GLAD.\n";

            shutdown();
            return false;
        }

        SDL_GL_SetSwapInterval(1);

        std::cout << "OpenGL renderer initialized.\n";

        const GLubyte* version = glGetString(GL_VERSION);

        if (version)
        {
            std::cout
                << "OpenGL version: "
                << version
                << '\n';
        }

        if (!createShaders())
        {
            shutdown();
            return false;
        }

        if (!createTriangleResources())
        {
            shutdown();
            return false;
        }

        m_initialized = true;

        std::cout << "Triangle renderer initialized.\n";

        return true;
    }

    bool Renderer::createShaders()
    {
        unsigned int vertexShader =
            glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(
            vertexShader,
            1,
            &vertexShaderSource,
            nullptr
        );

        glCompileShader(vertexShader);

        if (!checkShader(vertexShader, "vertex"))
        {
            glDeleteShader(vertexShader);
            return false;
        }

        unsigned int fragmentShader =
            glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(
            fragmentShader,
            1,
            &fragmentShaderSource,
            nullptr
        );

        glCompileShader(fragmentShader);

        if (!checkShader(fragmentShader, "fragment"))
        {
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

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (!checkProgram(m_shaderProgram))
        {
            glDeleteProgram(m_shaderProgram);
            m_shaderProgram = 0;

            return false;
        }

        return true;
    }

    bool Renderer::createTriangleResources()
    {
        const float vertices[] =
        {
             0.0f,  0.65f, 0.0f,
            -0.65f, -0.55f, 0.0f,
             0.65f, -0.55f, 0.0f
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

        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
            0.08f,
            0.10f,
            0.14f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);
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

        glUseProgram(0);
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

        std::cout
            << "OpenGL renderer shutdown.\n";
    }
}
