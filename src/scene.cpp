// Scene.cpp
#include "Scene.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "ShapeCreator.hpp"
#include "Transform.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
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

    // Kamera göstergelerini sil
    for(auto& camWithIndicator : m_camerasWithIndicators){
        delete camWithIndicator.indicator;
    }
    m_camerasWithIndicators.clear();
}

void Scene::addModel(Model* model)
{
    if(!model) return;
    m_modelList.push_back(model);
}

void Scene::removeModel(Model* model)
{
    auto it = std::find(m_modelList.begin(), m_modelList.end(), model);
    if(it != m_modelList.end()){
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
        m_activeCamera = cam;
    }
}

void Scene::addExtraCamera(Camera* cam)
{
    if(!cam) return;

    // Göstergenin modeli
    Model* indicator = new Model();
    indicator->setVertexArrayObject(
        ShapeCreator::createShape(ShapeTypes::Pyramid)
    );
    indicator->setShapeType(ShapeTypes::Pyramid);
    indicator->setShaderProgramName("TextureShader");
    indicator->setTextureName("hava.jpg");
    glLineWidth(4.f);
    indicator->setFillMode(GL_LINE);
    indicator->getTransform()->setScale(glm::vec3(0.2f)); // İstediğiniz boyutu ayarlayın
    indicator->setSelectable(false); // Seçilemez yapıldı
    addModel(indicator); // Göstergenin sahneye eklenmesi

    // Kamera ve göstergesini listeye ekleme
    m_camerasWithIndicators.push_back(CameraWithIndicator{cam, indicator});
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
