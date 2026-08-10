#include "Mesh.h"

namespace Pelvis
{

Mesh::Mesh()
    : m_vertexArray(0)
    , m_vertexBuffer(0)
    , m_vertexCount(0)
{
}

Mesh::~Mesh()
{
    destroy();
}

bool Mesh::create(
    const std::vector<Vertex>& vertices
)
{
    destroy();

    if (vertices.empty())
        return false;

    m_vertexCount =
        static_cast<GLsizei>(vertices.size());

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
        static_cast<GLsizeiptr>(
            vertices.size() * sizeof(Vertex)
        ),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
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

void Mesh::draw() const
{
    if (!isValid())
        return;

    glBindVertexArray(
        m_vertexArray
    );

    glDrawArrays(
        GL_TRIANGLES,
        0,
        m_vertexCount
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
}

bool Mesh::isValid() const
{
    return
        m_vertexArray != 0 &&
        m_vertexBuffer != 0 &&
        m_vertexCount > 0;
}

}
