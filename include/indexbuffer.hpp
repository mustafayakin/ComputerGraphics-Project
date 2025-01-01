#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

#include <glad/glad.h>
#include <cstddef>

namespace graf {
    class IndexBuffer {
    public:
        IndexBuffer();
        ~IndexBuffer();

        void create(const unsigned int* data, size_t size);

        void bind();
        void unbind();

        size_t getCount() const { return m_count; }

    private:
        GLuint m_id;
        size_t m_count;
    };
}

#endif // INDEXBUFFER_HPP
