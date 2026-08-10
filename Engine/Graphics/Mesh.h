#pragma once

#include <GL/glew.h>

namespace Pelvis
{

class Mesh
{
public:

    Mesh();
    ~Mesh();

    bool create(
        const float* vertices,
        unsigned int vertexCount
    );

    void draw() const;

    void destroy();

    bool isValid() const;

private:

    GLuint m_vertexArray;
    GLuint m_vertexBuffer;

    unsigned int m_vertexCount;
    bool m_initialized;
};

}
