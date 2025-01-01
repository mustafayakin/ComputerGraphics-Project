#include "vertexarrayobject.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"

namespace graf {

VertexArrayObject::VertexArrayObject()
    : m_id(0),
      m_vb(nullptr),
      m_ib(nullptr),
      m_stride(0)
{}

VertexArrayObject::~VertexArrayObject()
{
    if (m_id) {
        glDeleteVertexArrays(1, &m_id);
    }
}

void VertexArrayObject::create()
{
    glGenVertexArrays(1, &m_id);
}

void VertexArrayObject::bind()
{
    glBindVertexArray(m_id);
}

void VertexArrayObject::unbind()
{
    glBindVertexArray(0);
}

void VertexArrayObject::setVertexBuffer(VertexBuffer* vb)
{
    m_vb = vb;
}

void VertexArrayObject::setIndexBuffer(IndexBuffer* ib)
{
    m_ib = ib;
}

void VertexArrayObject::addVertexAttribute(VertexAttributeType attrType)
{
    m_attributes.push_back(attrType);
}

// Basit: Position(3 float) + TexCoord(2 float) -> stride 5 float
// Siz normal eklemek isterseniz 8 float'a çıkar
void VertexArrayObject::activateAttributes()
{
    bind();
    if (m_vb) m_vb->bind();
    if (m_ib) m_ib->bind();

    // Position(3 float) + TexCoord(2 float) = 5 floats
    m_stride = 5 * sizeof(float);

    int index = 0;
    int offset = 0;
    for (auto& attr : m_attributes) {
        if (attr == VertexAttributeType::Position) {
            // 3 float
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, m_stride, (void*)(offset));
            offset += 3 * sizeof(float);
            index++;
        }
        else if (attr == VertexAttributeType::Normal) {
            // Eğer normal kullansaydık 3 float
            // glEnableVertexAttribArray(index);
            // glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, m_stride, (void*)(offset));
            // offset += 3 * sizeof(float);
            // index++;
        }
        else if (attr == VertexAttributeType::Texture) {
            // 2 float
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, m_stride, (void*)(offset));
            offset += 2 * sizeof(float);
            index++;
        }
    }

    unbind();
}

void VertexArrayObject::draw()
{
    if (!m_ib) return;
    glDrawElements(GL_TRIANGLES, m_ib->getCount(), GL_UNSIGNED_INT, 0);
}

}
