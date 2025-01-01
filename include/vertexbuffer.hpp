#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <glad/glad.h>
#include <cstddef>

namespace graf {
    class VertexBuffer {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void create(const void* data, size_t size);

        void bind();
        void unbind();

    private:
        GLuint m_id;
    };
}

#endif // VERTEXBUFFER_HPP
