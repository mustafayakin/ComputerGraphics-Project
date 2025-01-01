#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <glm/glm.hpp>
#include "transform.hpp"
#include "shapetypes.hpp" // <-- enum class ShapeTypes

namespace graf {
    class VertexArrayObject;

    class Model {
    public:
        Model();
        ~Model();

        Transform* getTransform();

        void setVertexArrayObject(VertexArrayObject* vao);
        VertexArrayObject* getVertexArrayObject();

        void setShaderProgramName(const std::string& name);
        std::string getShaderProgramName();

        void setTextureName(const std::string& name);
        std::string getTextureName();

        glm::vec2& getTextureRepeat();

        void draw(const glm::mat4& MVP);
        void setFillMode(unsigned int mode);

        // Yeni: shape type
        void setShapeType(ShapeTypes st) { m_shapeType = st; }
        ShapeTypes getShapeType() const  { return m_shapeType; }

    private:
        Transform* m_transform;
        VertexArrayObject* m_vao;
        std::string m_programName;
        std::string m_textureName;
        glm::vec2   m_TextureRepeat;
        unsigned int m_fillType;

        // Eklendi
        ShapeTypes m_shapeType;
    };
}

#endif
