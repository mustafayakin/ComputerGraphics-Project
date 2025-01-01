#include "Camera.hpp"
#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream> // Opsiyonel: Hata ayıklama için

namespace graf {

// Varsayılan kurucu
Camera::Camera(float fov, float aspect, float nearVal, float farVal)
    : m_fov(fov),
      m_aspect(aspect),
      m_near(nearVal),
      m_far(farVal)
{
    m_Transform = new Transform();
    m_mtxProj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

// Kopyalama yapıcı (Derin kopyalama)
Camera::Camera(const Camera& other)
    : m_fov(other.m_fov),
      m_aspect(other.m_aspect),
      m_near(other.m_near),
      m_far(other.m_far),
      m_mtxProj(other.m_mtxProj)
{
    if(other.m_Transform) {
        m_Transform = new Transform(*other.m_Transform); // Derin kopyalama
    }
    else {
        m_Transform = nullptr;
    }
}

// Kopyalama atama operatörü (Derin kopyalama)
Camera& Camera::operator=(const Camera& other)
{
    if(this == &other)
        return *this; // Kendine atamayı önle

    // Mevcut Transform'u sil
    delete m_Transform;

    // Üyeleri kopyala
    m_fov = other.m_fov;
    m_aspect = other.m_aspect;
    m_near = other.m_near;
    m_far = other.m_far;
    m_mtxProj = other.m_mtxProj;

    // Transform'u derin kopyala
    if(other.m_Transform) {
        m_Transform = new Transform(*other.m_Transform);
    }
    else {
        m_Transform = nullptr;
    }

    return *this;
}

// Yıkıcı
Camera::~Camera()
{
    delete m_Transform;
}

glm::mat4 Camera::getProjMatrix()
{
    return m_mtxProj;
}

glm::mat4 Camera::getViewMatrix()
{
    auto pos = m_Transform->getPosition();
    auto rot = glm::inverse(m_Transform->getRotationMatrix());
    auto trans = glm::translate(glm::mat4(1.f), -pos);
    return rot * trans;
}

Transform* Camera::getTransform()
{
    return m_Transform;
}

void Camera::turn(float angle)
{
    if(m_Transform){
        auto eul = m_Transform->getEuler();
        eul.y += angle;
        m_Transform->setEuler(eul);
    }
}

void Camera::look(float angle)
{
    if(m_Transform){
        auto eul = m_Transform->getEuler();
        eul.x += angle;
        m_Transform->setEuler(eul);
    }
}

void Camera::setProjectionParams(float fov, float aspect, float nearVal, float farVal)
{
    m_fov = fov;
    m_aspect = aspect;
    m_near = nearVal;
    m_far = farVal;
    m_mtxProj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

} // namespace graf
