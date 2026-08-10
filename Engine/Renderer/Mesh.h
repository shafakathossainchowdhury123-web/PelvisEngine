#pragma once

#include <GL/glew.h>

#include <vector>

namespace Pelvis
{

struct Vertex
{
    float x;
    float y;
    float z;
};

class Mesh
{
public:

    Mesh();
    ~Mesh();

    bool create(
        const std::vector<Vertex>& vertices
    );

    void draw() const;

    void destroy();

    bool isValid() const;

private:

    GLuint m_vertexArray;
    GLuint m_vertexBuffer;

    GLsizei m_vertexCount;
};

}
