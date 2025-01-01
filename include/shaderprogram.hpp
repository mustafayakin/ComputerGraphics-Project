#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <map>

namespace graf {

    class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        bool create(const std::string& vsSource, const std::string& fsSource);
        void use();
        void unuse();

        // Uniform set fonksiyonları
        void setInt(const std::string& name, int value);
        void setMat4(const std::string& name, const glm::mat4& mat);
        void setVec2(const std::string& name, const glm::vec2& vec);

        GLuint getID() const { return m_id; }

    private:
        GLuint m_id;
    };

}

#endif // SHADERPROGRAM_HPP
