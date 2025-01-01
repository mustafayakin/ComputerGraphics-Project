// Model.cpp
#include "Model.hpp"
#include "transform.hpp"
#include "shadermanager.hpp"
#include "shaderprogram.hpp"
#include "texturemanager.hpp"
#include "vertexarrayobject.hpp"
#include <glad/glad.h>
#include <iostream>

namespace graf {

Model::Model()
{
    m_transform   = new Transform();
    m_vao         = nullptr;
    m_programName = "";
    m_textureName = "";
    m_TextureRepeat = glm::vec2(1.0f, 1.0f);
    m_fillType    = GL_FILL;
    m_shapeType   = ShapeTypes::Square; // varsayalım Square
    setSelectable(true); // Varsayılan olarak seçilebilir
}

Model::~Model()
{
    delete m_transform;
}

Transform* Model::getTransform()
{
    return m_transform;
}

void Model::setVertexArrayObject(VertexArrayObject* vao)
{
    m_vao = vao;
}

VertexArrayObject* Model::getVertexArrayObject()
{
    return m_vao;
}

void Model::setShaderProgramName(const std::string& name)
{
    m_programName = name;
}

std::string Model::getShaderProgramName()
{
    return m_programName;
}

void Model::setTextureName(const std::string& name)
{
    m_textureName = name;
}

std::string Model::getTextureName()
{
    return m_textureName;
}

glm::vec2& Model::getTextureRepeat()
{
    return m_TextureRepeat;
}

void Model::setFillMode(unsigned int mode)
{
    m_fillType = mode;
}

void Model::draw(const glm::mat4& MVP)
{
    if (!m_vao) return;

    // Shader programını al
    ShaderProgram* program = ShaderManager::getProgram(m_programName);
    if (!program) return;

    program->use();

    // Uniformlar
    program->setMat4("uWorldTransform", MVP * m_transform->getMatrix());
    program->setVec2("uTextureRepeat", m_TextureRepeat);

    // Texture bind
    TextureManager::activateTexture(m_textureName);

    // GL fill mode (ya da line)
    glPolygonMode(GL_FRONT_AND_BACK, m_fillType);

    // Çizim
    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();

    program->unuse();
}

// Yeni eklenen fonksiyonlar
bool Model::isSelectable() const {
    return m_selectable;
}

void Model::setSelectable(bool selectable) {
    m_selectable = selectable;
}

} // namespace graf
