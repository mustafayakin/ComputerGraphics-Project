// SceneMenu.cpp
#include "SceneMenu.hpp"
#include <imgui/imgui.h>
#include "Model.hpp"
#include "Camera.hpp"
#include "ShapeCreator.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"
#include "Transform.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

namespace graf {

static const char* shapeNames[] = {
    "Square",
    "Circle",
    "Cube",
    "Pyramid",
    "CutPyramid"
};

SceneMenu::SceneMenu()
    : m_selectedModelId(0),
      m_selectedTextureIndex(0),
      m_cameraIndex(0),
      m_fov(60.f),
      m_nearVal(0.1f),
      m_farVal(150.f),
      m_aspect(1.77f),
      m_selectorModel(nullptr),
      m_selectorAnimTime(0.f),
      m_cameraPos({0.f, 1.5f, 10.f}),
      m_cameraRot({0.f, 0.f, 0.f})
{
    // Textürler
    m_textureList.push_back("grass_texture.jpg");
    m_textureList.push_back("hava.jpg");
    m_textureList.push_back("rock.jpg");
    m_textureList.push_back("stone_wall_texture.png");
    m_textureList.push_back("container.jpg");
    m_textureList.push_back("white_texture.png");
    m_textureList.push_back("wall.jpg");

    // Selector modeli
    m_selectorModel = new Model();
    m_selectorModel->setVertexArrayObject(
        ShapeCreator::createShape(ShapeTypes::Pyramid)
    );
    m_selectorModel->setShapeType(ShapeTypes::Pyramid);
    m_selectorModel->setShaderProgramName("TextureShader");
    m_selectorModel->setTextureName("hava.jpg");

    // Piramit baş aşağı bakıyor
    m_selectorModel->getTransform()->setEuler({180.f, 0.f, 0.f});
    m_selectorModel->getTransform()->setScale(glm::vec3(0.6));
    m_selectorModel->setSelectable(false); // Seçilemez yapıldı
    addModel(m_selectorModel); // Sahneye ekleyin
}

SceneMenu::~SceneMenu()
{
    delete m_selectorModel;
    for (auto* cam : m_cameraList) {
        delete cam;
    }
}

void SceneMenu::addExtraCamera(Camera* cam)
{
    m_cameraList.push_back(cam);
}

void SceneMenu::updateSelector(float dt)
{
    m_selectorAnimTime += dt;
    float offsetY = 0.15f * sinf(m_selectorAnimTime * 1.f); // Daha yavaş ve az hareket

    if (m_modelList.empty()) return;
    if (m_selectedModelId < 0 || m_selectedModelId >= (int)m_modelList.size()) return;

    // Seçilen modelin sadece seçilebilir olup olmadığını kontrol edin
    int selectableCount = 0;
    Model* selectedModel = nullptr;
    for(auto* model : m_modelList){
        if(model->isSelectable()){
            if(selectableCount == m_selectedModelId){
                selectedModel = model;
                break;
            }
            selectableCount++;
        }
    }

    if(selectedModel == nullptr || selectedModel == m_selectorModel) return;

    auto pos = selectedModel->getTransform()->getPosition();
    m_selectorModel->getTransform()->setPosition({
        pos.x, pos.y + 2.0f + offsetY, pos.z // Daha az yukarıya çıkar
    });
}

void SceneMenu::renderGui()
{
    updateSelector(0.016f);

    ImGui::Begin("Model Control");

    // Seçilebilir modelleri listele
    std::vector<Model*> selectableModels;
    for(auto* model : m_modelList){
        if(model->isSelectable()){
            selectableModels.push_back(model);
        }
    }

    // Model seçimi
    int maxCount = static_cast<int>(selectableModels.size()) - 1;
    if (maxCount < 0) maxCount = 0;
    if (m_selectedModelId > maxCount) m_selectedModelId = maxCount;
    if (ImGui::SliderInt("Model Id", &m_selectedModelId, 0, maxCount)) {
        if (!selectableModels.empty()) {
            auto sel = selectableModels[m_selectedModelId];
            std::string tName = sel->getTextureName();
            int found = 0;
            for (int i = 0; i < static_cast<int>(m_textureList.size()); i++) {
                if (m_textureList[i] == tName) {
                    found = i;
                    break;
                }
            }
            m_selectedTextureIndex = found;
        }
    }

    // Shape tipi değiştirme
    if (!selectableModels.empty()) {
        auto& selModel = selectableModels[m_selectedModelId];
        int currentShapeIndex = static_cast<int>(selModel->getShapeType());
        if (ImGui::Combo("Shape", &currentShapeIndex, shapeNames, IM_ARRAYSIZE(shapeNames))) {
            selModel->setVertexArrayObject(
                ShapeCreator::createShape(static_cast<ShapeTypes>(currentShapeIndex))
            );
            selModel->setShapeType(static_cast<ShapeTypes>(currentShapeIndex));
        }
    }

    ImGui::Spacing();
    ImGui::Text("Textures");
    ImGui::Separator();

    if (ImGui::BeginCombo("##TextureCombo", m_textureList[m_selectedTextureIndex].c_str())) {
        for (int i = 0; i < static_cast<int>(m_textureList.size()); i++) {
            bool isSelected = (i == m_selectedTextureIndex);
            if (ImGui::Selectable(m_textureList[i].c_str(), isSelected)) {
                m_selectedTextureIndex = i;
                if (!selectableModels.empty()) {
                    auto selModel = selectableModels[m_selectedModelId];
                    selModel->setTextureName(m_textureList[i]);
                }
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    static float texRep[2] = {1.f, 1.f};
    if (!selectableModels.empty()) {
        auto& rep = selectableModels[m_selectedModelId]->getTextureRepeat();
        texRep[0] = rep.x;
        texRep[1] = rep.y;
    }
    if (ImGui::DragFloat2("Repeat", texRep, 0.01f, 0.f, 10.f)) {
        if (!selectableModels.empty()) {
            auto& r = selectableModels[m_selectedModelId]->getTextureRepeat();
            r.x = texRep[0];
            r.y = texRep[1];
        }
    }

    ImGui::Spacing();
    ImGui::Text("Transform");
    ImGui::Separator();

    static float pp[3], ee[3], ss[3];
    if (!selectableModels.empty()) {
        auto selTr = selectableModels[m_selectedModelId]->getTransform();
        auto p = selTr->getPosition();
        auto e = selTr->getEuler();
        auto s = selTr->getScale();
        pp[0] = p.x; pp[1] = p.y; pp[2] = p.z;
        ee[0] = e.x; ee[1] = e.y; ee[2] = e.z;
        ss[0] = s.x; ss[1] = s.y; ss[2] = s.z;

        if (ImGui::DragFloat3("Pos", pp, 0.05f)) {
            selTr->setPosition({pp[0], pp[1], pp[2]});
        }
        if (ImGui::DragFloat3("Rot", ee, 0.5f)) {
            selTr->setEuler({ee[0], ee[1], ee[2]});
        }
        if (ImGui::DragFloat3("Scale", ss, 0.01f, 0.01f, 999.f)) {
            selTr->setScale({ss[0], ss[1], ss[2]});
        }
    }

    ImGui::Spacing();
    ImGui::Text("Camera Selection");
    ImGui::Separator();

    int totalCam = 1 + static_cast<int>(m_cameraList.size());
    if (m_cameraIndex >= totalCam) m_cameraIndex = totalCam - 1;

    bool camChanged = ImGui::SliderInt("Camera##Index", &m_cameraIndex, 0, totalCam - 1);

    Camera* currentCam = nullptr;
    if (m_cameraIndex == 0) {
        currentCam = m_activeCamera;
    } else {
        int idx = m_cameraIndex - 1;
        if (idx >= 0 && idx < static_cast<int>(m_cameraList.size())) {
            currentCam = m_cameraList[idx];
        }
    }

    if (currentCam && currentCam != m_activeCamera) {
        setActiveCamera(currentCam);
        camChanged = true;
    }

    if (camChanged && currentCam) {
        m_cameraPos = currentCam->getTransform()->getPosition();
        m_cameraRot = currentCam->getTransform()->getEuler();
    }

    if (currentCam) {
        if (ImGui::DragFloat3("Cam Pos##", &m_cameraPos[0], 0.05f)) {
            currentCam->getTransform()->setPosition(m_cameraPos);
        }
        if (ImGui::DragFloat3("Cam Rot##", &m_cameraRot[0], 0.5f)) {
            currentCam->getTransform()->setEuler(m_cameraRot);
        }
    }

    ImGui::Spacing();
    ImGui::Text("Model (Projection)");
    ImGui::Separator();
    bool changed = false;
    changed |= ImGui::DragFloat("activeFOV", &m_fov, 0.1f, 1.f, 179.f);
    changed |= ImGui::DragFloat("activeNear", &m_nearVal, 0.01f, 0.01f, 10.f);
    changed |= ImGui::DragFloat("activeFar", &m_farVal, 1.f, 1.f, 150.f);
    changed |= ImGui::DragFloat("activeAspect", &m_aspect, 0.01f, 0.1f, 4.f);

    if (changed && currentCam) {
        currentCam->setProjectionParams(m_fov, m_aspect, m_nearVal, m_farVal);
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(900, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Creation");

    // Model Creation
    ImGui::Text("Model Creation");
    ImGui::Separator();

    static int selectedShapeIndex = 0;
    if (ImGui::BeginCombo("Shapes", shapeNames[selectedShapeIndex])) {
        for (int i = 0; i < IM_ARRAYSIZE(shapeNames); i++) {
            bool isSelected = (selectedShapeIndex == i);
            if (ImGui::Selectable(shapeNames[i], isSelected)) {
                selectedShapeIndex = i;
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    static char selectedTexture[64] = "container.jpg";
    if (ImGui::BeginCombo("Textures", selectedTexture)) {
        for (const auto& tex : m_textureList) {
            bool isSelected = (tex == selectedTexture);
            if (ImGui::Selectable(tex.c_str(), isSelected)) {
                strcpy(selectedTexture, tex.c_str());
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    static float modelRepeat[2] = {1.0f, 1.0f};
    ImGui::DragFloat2("Repeat", modelRepeat, 0.01f, 0.f, 10.f);

    if (ImGui::Button("Create Model")) {
        auto newModel = new Model();
        newModel->setVertexArrayObject(
            ShapeCreator::createShape(static_cast<ShapeTypes>(selectedShapeIndex))
        );
        newModel->setShapeType(static_cast<ShapeTypes>(selectedShapeIndex));
        newModel->setShaderProgramName("TextureShader");
        newModel->setTextureName(selectedTexture);
        newModel->getTextureRepeat() = {modelRepeat[0], modelRepeat[1]};
        addModel(newModel);
    }
    ImGui::Spacing();
    ImGui::Text("Camera Creation");
    ImGui::Separator();

    static float camFOV = 60.f;
    static float camNear = 0.1f;
    static float camFar = 150.f;
    static float camWidth = 800.f;
    static float camHeight = 600.f;

    ImGui::DragFloat("FOV", &camFOV, 0.1f, 1.f, 179.f);
    ImGui::DragFloat("Near", &camNear, 0.01f, 0.01f, 10.f);
    ImGui::DragFloat("Far", &camFar, 1.f, 1.f, 5000.f);
    ImGui::DragFloat("Width", &camWidth, 1.f, 10.f, 1920.f);
    ImGui::DragFloat("Height", &camHeight, 1.f, 10.f, 1080.f);

    if (ImGui::Button("Create Camera")) {
        auto* newCam = new Camera(camFOV, camWidth / camHeight, camNear, camFar);
        m_cameraList.push_back(newCam);
    }

    ImGui::End();
}

} // namespace graf
