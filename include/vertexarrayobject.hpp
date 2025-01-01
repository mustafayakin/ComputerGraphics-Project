#ifndef VERTEXARRAYOBJECT_HPP
#define VERTEXARRAYOBJECT_HPP

#include <glad/glad.h>
#include <vector>
#include "vertextypes.hpp"

namespace graf {
    class VertexBuffer;
    class IndexBuffer;

    class VertexArrayObject {
    public:
        VertexArrayObject();
        ~VertexArrayObject();

        void create();

        void bind();
        void unbind();

        void setVertexBuffer(VertexBuffer* vb);
        void setIndexBuffer(IndexBuffer* ib);

        void addVertexAttribute(VertexAttributeType attrType);
        void activateAttributes();

        void draw();

    private:
        GLuint m_id;
        VertexBuffer* m_vb;
        IndexBuffer* m_ib;
        std::vector<VertexAttributeType> m_attributes;
        int m_stride;
    };

}

#endif // VERTEXARRAYOBJECT_HPP
