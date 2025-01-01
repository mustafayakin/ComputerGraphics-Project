#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace graf {

    class Transform {
    public:
        Transform();

        // Pozisyon
        void setPosition(const glm::vec3& pos);
        glm::vec3 getPosition();

        // Euler açılar
        void setEuler(const glm::vec3& euler);
        glm::vec3& getEuler();

        // Hareket fonksiyonları
        void moveForward();
        void moveBackward();
        void moveLeft();
        void moveRight();

        // Ölçek
        void setScale(const glm::vec3& scale);
        glm::vec3 getScale() const;

        // +Z yönü (look direction)
        glm::vec3 getLook();

        // Rotasyon matrisi
        glm::mat4 getRotationMatrix();
        // Dünya matrisi
        glm::mat4 getMatrix();

    private:
        void update();

        glm::vec3 m_Position;
        glm::vec3 m_Euler;
        glm::vec3 m_Scale;

        glm::mat4 m_MtxWorld;      // T * R * S
        glm::mat4 m_MtxRotation;   // salt R
    };

}

#endif // TRANSFORM_HPP
