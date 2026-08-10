#include "Mesh.h"

namespace Pelvis
{

Mesh::Mesh()
    : m_vertexArray(0)
    , m_vertexBuffer(0)
    , m_vertexCount(0)
    , m_initialized(false)
{
}

Mesh::~Mesh()
{
    destroy();
}

bool Mesh::create(
    const float* vertices,
    unsigned int vertexCount
)
{
    if (!vertices || vertexCount == 0)
        return false;

    destroy();

    m_vertexCount = vertexCount;

    glGenVertexArrays(
        1,
        &m_vertexArray
    );

    glGenBuffers(
        1,
        &m_vertexBuffer
    );

    if (!m_vertexArray || !m_vertexBuffer)
    {
        destroy();
        return false;
    }

    glBindVertexArray(
        m_vertexArray
    );

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_vertexBuffer
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * vertexCount * 3,
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

    m_initialized = true;

    return true;
}

void Mesh::draw() const
{
    if (!m_initialized)
        return;

    glBindVertexArray(
        m_vertexArray
    );

    glDrawArrays(
        GL_TRIANGLES,
        0,
        static_cast<GLsizei>(m_vertexCount)
    );

    glBindVertexArray(0);
}

void Mesh::destroy()
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

    m_vertexCount = 0;
    m_initialized = false;
}

bool Mesh::isValid() const
{
    return m_initialized;
}

}
