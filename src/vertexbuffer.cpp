#include "vertexbuffer.hpp"

namespace graf {

VertexBuffer::VertexBuffer()
    : m_id(0)
{
}

VertexBuffer::~VertexBuffer()
{
    if (m_id) {
        glDeleteBuffers(1, &m_id);
    }
}

void VertexBuffer::create(const void* data, size_t size)
{
    if (!m_id) {
        glGenBuffers(1, &m_id);
    }
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbind();
}

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
