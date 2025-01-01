#include "Scene.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include <algorithm>

namespace graf {

Scene::Scene() {
    m_activeCamera = new Camera();
}

Scene::~Scene() {
    delete m_activeCamera;
    for(auto m : m_modelList){
        delete m;
    }
    m_modelList.clear();
}

void Scene::addModel(Model* model)
{
    if(!model) return;
    m_modelList.push_back(model);
}

void Scene::removeModel(Model* model)
{
    auto it= std::find(m_modelList.begin(), m_modelList.end(), model);
    if(it!=m_modelList.end()){
        m_modelList.erase(it);
        delete model;
    }
}

Camera* Scene::getActiveCamera()
{
    return m_activeCamera;
}

void Scene::setActiveCamera(Camera* cam)
{
    if(cam) {
        m_activeCamera=cam;
    }
}

void Scene::render(bool drawGui)
{
    for(auto* model : m_modelList){
        if(m_activeCamera){
            model->draw(m_activeCamera->getProjMatrix() * m_activeCamera->getViewMatrix());
        }
    }
    if(drawGui) {
        renderGui();
    }
}

void Scene::renderGui()
{
    // Boş, override edilebilir (SceneMenu vs.)
}

} // namespace graf
