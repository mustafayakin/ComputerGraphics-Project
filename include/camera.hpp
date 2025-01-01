#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

namespace graf {
    class Transform;

    class Camera {
    public:
        // Varsayılan kurucu
        Camera(float fov = 60.f, float aspect = 1.77f, float nearVal = 0.1f, float farVal = 1000.f);

        // Kopyalama yapıcı
        Camera(const Camera& other);

        // Kopyalama atama operatörü
        Camera& operator=(const Camera& other);

        // Yıkıcı
        ~Camera();

        // Projeksiyon matrisi al
        glm::mat4 getProjMatrix();

        // Görünüm matrisi al
        glm::mat4 getViewMatrix();

        // Transform nesnesini al
        Transform* getTransform();

        // Kamerayı döndür
        void turn(float angle);

        // Kamerayı bakma yönünü değiştir
        void look(float angle);

        // Projeksiyon parametrelerini ayarla
        void setProjectionParams(float fov, float aspect, float nearVal, float farVal);

    private:
        Transform* m_Transform;    // Kameranın dönüşüm bilgisi
        glm::mat4  m_mtxProj;      // Projeksiyon matrisi
        float m_fov;                // Görüş alanı
        float m_aspect;             // En boy oranı
        float m_near;               // Yakın kesim düzlemi
        float m_far;                // Uzak kesim düzlemi
    };

} // namespace graf

#endif // CAMERA_HPP
