#include "indexbuffer.hpp"

namespace graf {

IndexBuffer::IndexBuffer()
    : m_id(0),
      m_count(0)
{
}

IndexBuffer::~IndexBuffer()
{
    if (m_id) {
        glDeleteBuffers(1, &m_id);
    }
}

void IndexBuffer::create(const unsigned int* data, size_t size)
{
    if (!m_id) {
        glGenBuffers(1, &m_id);
    }
    m_count = size / sizeof(unsigned int);

    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbind();
}

void IndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
