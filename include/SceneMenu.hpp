// SceneMenu.hpp
#ifndef SCENEMENU_HPP
#define SCENEMENU_HPP

#include "Scene.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace graf {

    class Camera;
    class Model;

    class SceneMenu : public Scene {
    public:
        SceneMenu();
        ~SceneMenu();

        void renderGui() override;
        void addExtraCamera(Camera* cam) override;

        void updateSelector(float dt);

    private:
        int   m_selectedModelId;

        std::vector<std::string> m_textureList;
        int   m_selectedTextureIndex;

        int   m_cameraIndex;

        float m_fov;
        float m_nearVal;
        float m_farVal;
        float m_aspect;

        glm::vec3 m_cameraPos;  // Kamera pozisyonunu takip etmek için
        glm::vec3 m_cameraRot;  // Kamera rotasyonunu takip etmek için

        // “Selector” piramit
        Model* m_selectorModel;
        float  m_selectorAnimTime;
    };

} // namespace graf

#endif // SCENEMENU_HPP
