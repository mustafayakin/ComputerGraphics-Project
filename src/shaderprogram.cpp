#include "shaderprogram.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace graf {

static GLuint compileShader(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    const char* cstr = source.c_str();
    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "[Shader Error] " << log << std::endl;
        return 0;
    }
    return shader;
}

ShaderProgram::ShaderProgram()
    : m_id(0)
{}

ShaderProgram::~ShaderProgram()
{
    if (m_id) {
        glDeleteProgram(m_id);
    }
}

bool ShaderProgram::create(const std::string& vsSource, const std::string& fsSource)
{
    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

    if (!vs || !fs) return false;

    m_id = glCreateProgram();
    glAttachShader(m_id, vs);
    glAttachShader(m_id, fs);
    glLinkProgram(m_id);

    GLint success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(m_id, 512, nullptr, log);
        std::cerr << "[Link Error] " << log << std::endl;
        glDeleteProgram(m_id);
        m_id = 0;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    return (m_id != 0);
}

void ShaderProgram::use()
{
    glUseProgram(m_id);
}

void ShaderProgram::unuse()
{
    glUseProgram(0);
}

void ShaderProgram::setInt(const std::string& name, int value)
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    if (loc >= 0) {
        glUniform1i(loc, value);
    }
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat)
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    if (loc >= 0) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& vec)
{
    GLint loc = glGetUniformLocation(m_id, name.c_str());
    if (loc >= 0) {
        glUniform2fv(loc, 1, &vec.x);
    }
}

}
