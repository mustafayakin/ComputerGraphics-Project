#define GLM_ENABLE_EXPERIMENTAL
#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

namespace graf {

Transform::Transform()
{
    m_Position = glm::vec3(0.f);
    m_Euler    = glm::vec3(0.f);
    m_Scale    = glm::vec3(1.f);
    m_MtxWorld = glm::mat4(1.f);
    m_MtxRotation = glm::mat4(1.f);
    update();
}

void Transform::setPosition(const glm::vec3& pos)
{
    m_Position = pos;
    update();
}

glm::vec3 Transform::getPosition()
{
    return m_Position;
}

void Transform::setEuler(const glm::vec3& euler)
{
    m_Euler = euler;
    float rx = glm::radians(m_Euler.x);
    float ry = glm::radians(m_Euler.y);
    float rz = glm::radians(m_Euler.z);

    m_MtxRotation = glm::eulerAngleXYZ(rx, ry, rz);
    update();
}

glm::vec3& Transform::getEuler()
{
    return m_Euler;
}

void Transform::moveForward()
{
    auto look = glm::vec3(m_MtxRotation[2][0], m_MtxRotation[2][1], m_MtxRotation[2][2]);
    m_Position += glm::normalize(look);
    update();
}

void Transform::moveBackward()
{
    auto look = glm::vec3(m_MtxRotation[2][0], m_MtxRotation[2][1], m_MtxRotation[2][2]);
    m_Position -= glm::normalize(look);
    update();
}

void Transform::moveLeft()
{
    auto right = glm::vec3(m_MtxRotation[0][0], m_MtxRotation[0][1], m_MtxRotation[0][2]);
    m_Position -= glm::normalize(right);
    update();
}

void Transform::moveRight()
{
    auto right = glm::vec3(m_MtxRotation[0][0], m_MtxRotation[0][1], m_MtxRotation[0][2]);
    m_Position += glm::normalize(right);
    update();
}

void Transform::setScale(const glm::vec3& scale)
{
    m_Scale = scale;
    update();
}

glm::vec3 Transform::getScale() const
{
    return m_Scale;
}

glm::vec3 Transform::getLook()
{
    glm::vec3 look(
        m_MtxRotation[2][0],
        m_MtxRotation[2][1],
        m_MtxRotation[2][2]
    );
    return glm::normalize(look);
}

glm::mat4 Transform::getRotationMatrix()
{
    return m_MtxRotation;
}

glm::mat4 Transform::getMatrix()
{
    return m_MtxWorld;
}

void Transform::update()
{
    glm::mat4 T = glm::translate(glm::mat4(1.f), m_Position);
    glm::mat4 S = glm::scale(glm::mat4(1.f), m_Scale);
    m_MtxWorld = T * m_MtxRotation * S;
}

} // namespace graf
