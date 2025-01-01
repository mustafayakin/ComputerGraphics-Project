#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <string>

namespace graf {
    class Model;
    class Camera;

    class Scene {
    public:
        Scene();
        virtual ~Scene();

        void addModel(Model* model);
        void removeModel(Model* model);

        Camera* getActiveCamera();
        void setActiveCamera(Camera* cam);

        virtual void render(bool drawGui = true);

    protected:
        virtual void renderGui();

        Camera* m_activeCamera;
        std::vector<Model*> m_modelList;
    };

}

#endif
