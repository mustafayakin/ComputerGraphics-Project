#include "shapecreator.hpp"
#include "vertexarrayobject.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertextypes.hpp" // struct Vertex
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

namespace graf {

ShapeCreator* ShapeCreator::m_Instance = nullptr;

VertexArrayObject* ShapeCreator::createShape(ShapeTypes shape)
{
    if (!m_Instance) {
        m_Instance = new ShapeCreator();
    }

    switch (shape) {
        case ShapeTypes::Square:
            return createSquare();
        case ShapeTypes::Circle:
            return createCircle(10);
        case ShapeTypes::Cube:
            return createCube();
        case ShapeTypes::Pyramid:
            return createPyramid();
        case ShapeTypes::CutPyramid:
            return createCutPyramid();
        default:
            return nullptr;
    }
}

VertexArrayObject* ShapeCreator::createSquare()
{
    if (m_Instance->m_vaoMap.count(ShapeTypes::Square) > 0) {
        return m_Instance->m_vaoMap[ShapeTypes::Square];
    }

    float vertices[] = {
        // x,   y,   z,    u,   v
       -0.5f,  0.5f, 0.f,  0.f, 1.f,
        0.5f,  0.5f, 0.f,  1.f, 1.f,
        0.5f, -0.5f, 0.f,  1.f, 0.f,
       -0.5f, -0.5f, 0.f,  0.f, 0.f
    };
    unsigned int indices[] = {0,1,2, 2,3,0};

    VertexBuffer* vb = new VertexBuffer();
    vb->create(vertices, sizeof(vertices));

    IndexBuffer* ib = new IndexBuffer();
    ib->create(indices, sizeof(indices));

    VertexArrayObject* vao = new VertexArrayObject();
    vao->create();
    vao->setVertexBuffer(vb);
    vao->setIndexBuffer(ib);

    vao->addVertexAttribute(VertexAttributeType::Position);
    vao->addVertexAttribute(VertexAttributeType::Texture);
    vao->activateAttributes();
    vao->unbind();

    m_Instance->m_vaoMap[ShapeTypes::Square] = vao;
    return vao;
}

VertexArrayObject* ShapeCreator::createCircle(int anglesInDegrees)
{
    if (m_Instance->m_vaoMap.count(ShapeTypes::Circle) > 0) {
        return m_Instance->m_vaoMap[ShapeTypes::Circle];
    }
    std::vector<Vertex> vertices;
    int vertexCount = 360 / anglesInDegrees; 
    vertices.reserve(vertexCount);

    for(int i=0; i<vertexCount; i++)
    {
        float deg = i * (float)anglesInDegrees; 
        float rad = glm::radians(deg);
        Vertex v;
        v.position.x = std::cos(rad);
        v.position.y = std::sin(rad);
        v.position.z = 0.f;
        v.texture.x = 0.5f + 0.5f*std::cos(rad);
        v.texture.y = 0.5f + 0.5f*std::sin(rad);
        vertices.push_back(v);
    }

    std::vector<unsigned int> indices;
    for(int i=1; i<vertexCount-1; i++)
    {
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i);
    }

    VertexBuffer* vb = new VertexBuffer();
    vb->create(vertices.data(), vertices.size()*sizeof(Vertex));

    IndexBuffer* ib = new IndexBuffer();
    ib->create(indices.data(), indices.size()*sizeof(unsigned int));

    VertexArrayObject* vao = new VertexArrayObject();
    vao->create();
    vao->setVertexBuffer(vb);
    vao->setIndexBuffer(ib);
    vao->addVertexAttribute(VertexAttributeType::Position);
    vao->addVertexAttribute(VertexAttributeType::Texture);
    vao->activateAttributes();
    vao->unbind();

    m_Instance->m_vaoMap[ShapeTypes::Circle] = vao;
    return vao;
}

VertexArrayObject* ShapeCreator::createCube()
{
   if (m_Instance->m_vaoMap.count(ShapeTypes::Cube) > 0) {
        return m_Instance->m_vaoMap[ShapeTypes::Cube];
    }

    // 8 köşe, ama her yüz için 4 vertex (UV mapping)
    // => 24 vertex
    glm::vec3 pos[] = {
       // front face
       {-0.5f,  0.5f,  0.5f}, 
       { 0.5f,  0.5f,  0.5f},
       { 0.5f, -0.5f,  0.5f},
       {-0.5f, -0.5f,  0.5f},

       // back face
       {-0.5f,  0.5f, -0.5f},
       { 0.5f,  0.5f, -0.5f},
       { 0.5f, -0.5f, -0.5f},
       {-0.5f, -0.5f, -0.5f}
    };
    // 24 vertex
    Vertex vertices[24];
    // front
    vertices[0].position = pos[0];  vertices[0].texture = {0.f,1.f};
    vertices[1].position = pos[1];  vertices[1].texture = {1.f,1.f};
    vertices[2].position = pos[2];  vertices[2].texture = {1.f,0.f};
    vertices[3].position = pos[3];  vertices[3].texture = {0.f,0.f};

    // right
    vertices[4].position = pos[1];  vertices[4].texture = {0.f,1.f};
    vertices[5].position = pos[5];  vertices[5].texture = {1.f,1.f};
    vertices[6].position = pos[6];  vertices[6].texture = {1.f,0.f};
    vertices[7].position = pos[2];  vertices[7].texture = {0.f,0.f};

    // top
    vertices[8].position  = pos[4]; vertices[8].texture  = {0.f,1.f};
    vertices[9].position  = pos[5]; vertices[9].texture  = {1.f,1.f};
    vertices[10].position = pos[1]; vertices[10].texture = {1.f,0.f};
    vertices[11].position = pos[0]; vertices[11].texture = {0.f,0.f};

    // left
    vertices[12].position = pos[4]; vertices[12].texture = {0.f,1.f};
    vertices[13].position = pos[0]; vertices[13].texture = {1.f,1.f};
    vertices[14].position = pos[3]; vertices[14].texture = {1.f,0.f};
    vertices[15].position = pos[7]; vertices[15].texture = {0.f,0.f};

    // back
    vertices[16].position = pos[5]; vertices[16].texture = {0.f,1.f};
    vertices[17].position = pos[4]; vertices[17].texture = {1.f,1.f};
    vertices[18].position = pos[7]; vertices[18].texture = {1.f,0.f};
    vertices[19].position = pos[6]; vertices[19].texture = {0.f,0.f};

    // bottom
    vertices[20].position = pos[3]; vertices[20].texture = {0.f,1.f};
    vertices[21].position = pos[2]; vertices[21].texture = {1.f,1.f};
    vertices[22].position = pos[6]; vertices[22].texture = {1.f,0.f};
    vertices[23].position = pos[7]; vertices[23].texture = {0.f,0.f};

    // index: 6 face * 2 tri = 12 tri => 36 index
    std::vector<unsigned int> indices;
    indices.reserve(36);
    for (int i = 0; i < 6; i++) {
        int base = i * 4;
        indices.push_back(base+0);
        indices.push_back(base+2);
        indices.push_back(base+1);

        indices.push_back(base+0);
        indices.push_back(base+3);
        indices.push_back(base+2);
    }

    VertexBuffer* vb = new VertexBuffer();
    vb->create(vertices, sizeof(vertices));

    IndexBuffer* ib = new IndexBuffer();
    ib->create(indices.data(), indices.size() * sizeof(unsigned int));

    VertexArrayObject* va = new VertexArrayObject();
    va->create();
    va->setVertexBuffer(vb);
    va->setIndexBuffer(ib);

    va->addVertexAttribute(VertexAttributeType::Position);
    va->addVertexAttribute(VertexAttributeType::Texture);
    va->activateAttributes();
    va->unbind();

    m_Instance->m_vaoMap[ShapeTypes::Cube] = va;
    return va;
}

VertexArrayObject* ShapeCreator::createPyramid()
{
     if (m_Instance->m_vaoMap.count(ShapeTypes::Pyramid) > 0) {
        return m_Instance->m_vaoMap[ShapeTypes::Pyramid];
    }

    // 5 nokta: 4 alt, 1 tepe
    glm::vec3 positions[] = {
        {-0.5f, 0.f, -0.5f}, // 0
        { 0.5f, 0.f, -0.5f}, // 1
        { 0.5f, 0.f,  0.5f}, // 2
        {-0.5f, 0.f,  0.5f}, // 3
        { 0.0f, 1.f,  0.0f}  // 4 (tepe)
    };
    // 5 Vertex
    std::vector<Vertex> vertices(5);
    // alt taban uv
    vertices[0].position = positions[0];  vertices[0].texture={0.f,0.f};
    vertices[1].position = positions[1];  vertices[1].texture={1.f,0.f};
    vertices[2].position = positions[2];  vertices[2].texture={1.f,1.f};
    vertices[3].position = positions[3];  vertices[3].texture={0.f,1.f};

    // tepe
    vertices[4].position = positions[4];  vertices[4].texture={0.5f,0.5f};

    // index: alt taban 2 tri + 4 yan yüz
    std::vector<unsigned int> indices;

    // alt taban
    indices.push_back(0); 
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // yan yüzler: (i, i+1, tepe)
    for (int i=0; i<4; i++){
        int next=(i+1)%4;
        indices.push_back(i);
        indices.push_back(next);
        indices.push_back(4);
    }

    VertexBuffer* vb = new VertexBuffer();
    vb->create(vertices.data(), vertices.size()*sizeof(Vertex));

    IndexBuffer* ib = new IndexBuffer();
    ib->create(indices.data(), indices.size()*sizeof(unsigned int));

    VertexArrayObject* va = new VertexArrayObject();
    va->create();
    va->setVertexBuffer(vb);
    va->setIndexBuffer(ib);

    va->addVertexAttribute(VertexAttributeType::Position);
    va->addVertexAttribute(VertexAttributeType::Texture);
    va->activateAttributes();
    va->unbind();

    m_Instance->m_vaoMap[ShapeTypes::Pyramid] = va;
    return va;
}

VertexArrayObject* ShapeCreator::createCutPyramid()
{
    if (m_Instance->m_vaoMap.count(ShapeTypes::CutPyramid) > 0) {
        return m_Instance->m_vaoMap[ShapeTypes::CutPyramid];
    }

    // Alt taban: 4 nokta
    glm::vec3 bottom[] = {
        {-0.5f, 0.f, -0.5f}, // 0
        { 0.5f, 0.f, -0.5f}, // 1
        { 0.5f, 0.f,  0.5f}, // 2
        {-0.5f, 0.f,  0.5f}  // 3
    };
    // Üst taban (küçük) 4 nokta
    glm::vec3 top[] = {
        {-0.25f, 1.f, -0.25f}, // 4
        { 0.25f, 1.f, -0.25f}, // 5
        { 0.25f, 1.f,  0.25f}, // 6
        {-0.25f, 1.f,  0.25f}  // 7
    };
    // (opsiyonel) tepe noktası yok veya siz isterseniz eklersiniz
    // "cut pyramid" => trapezoidal prism gibi?

    // 8 vertex
    std::vector<Vertex> vertices(8);
    // bottom
    for (int i=0; i<4; i++){
        vertices[i].position=bottom[i];
        vertices[i].texture={ (float)(i%2), (float)(i/2) };
    }
    // top
    for (int i=0; i<4; i++){
        vertices[i+4].position=top[i];
        vertices[i+4].texture={ (float)(i%2), (float)(i/2) };
    }

    // index
    std::vector<unsigned int> indices;

    // alt taban
    indices.push_back(0); 
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // üst taban
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);

    // yan yüzler => 4 side
    // i, next, i+4, next+4
    for (int i=0; i<4; i++){
        int next=(i+1)%4;
        indices.push_back(i);
        indices.push_back(next);
        indices.push_back(i+4);

        indices.push_back(next);
        indices.push_back(next+4);
        indices.push_back(i+4);
    }

    VertexBuffer* vb = new VertexBuffer();
    vb->create(vertices.data(), vertices.size()*sizeof(Vertex));

    IndexBuffer* ib = new IndexBuffer();
    ib->create(indices.data(), indices.size()*sizeof(unsigned int));

    VertexArrayObject* va = new VertexArrayObject();
    va->create();
    va->setVertexBuffer(vb);
    va->setIndexBuffer(ib);

    va->addVertexAttribute(VertexAttributeType::Position);
    va->addVertexAttribute(VertexAttributeType::Texture);
    va->activateAttributes();
    va->unbind();

    m_Instance->m_vaoMap[ShapeTypes::CutPyramid] = va;
    return va;
}

} // namespace graf
