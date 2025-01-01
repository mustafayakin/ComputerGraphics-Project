#ifndef VERTEXTYPES_HPP
#define VERTEXTYPES_HPP

#include <glm/glm.hpp>

namespace graf {

// Hangi attrib’lerin olduğunu tanımlıyoruz
enum class VertexAttributeType {
    Position,
    Normal,
    Texture
};

// Sizin ShapeCreator kodunuzda "Vertex" diyerek kullanacağınız struct.
// (Position + Texture. Normal isterseniz ekleyebilirsiniz.)
struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture;
    // glm::vec3 normal; // eğer aydınlatma yapacaksanız ekleyin
};

} // namespace graf

#endif // VERTEXTYPES_HPP
