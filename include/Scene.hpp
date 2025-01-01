// Scene.hpp
#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace graf {
    class Model;
    class Camera;

    // Yeni: Kamera ve göstergesini bir arada tutacak yapı
    struct CameraWithIndicator {
        Camera* camera;
        Model* indicator;
    };

    class Scene {
    public:
        Scene();
        virtual ~Scene();

        void addModel(Model* model);
        void removeModel(Model* model);

        Camera* getActiveCamera();
        void setActiveCamera(Camera* cam);

        // Yeni: Ekstra kamera eklemek için metot
        virtual void addExtraCamera(Camera* cam);

        virtual void render(bool drawGui = true);

    protected:
        virtual void renderGui();

        Camera* m_activeCamera;
        std::vector<Model*> m_modelList;

        // Yeni: Kamera ve göstergelerini tutan liste
        std::vector<CameraWithIndicator> m_camerasWithIndicators;
    };

}

#endif // SCENE_HPP
