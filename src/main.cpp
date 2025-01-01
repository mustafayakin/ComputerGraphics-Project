// main.cpp

#define GLFW_INCLUDE_NONE

#include "GLWindow.hpp"
#include "SceneMenu.hpp"    // Türemiş sahne
#include "Camera.hpp"
#include "Model.hpp"
#include "ShapeCreator.hpp"
#include "Transform.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    // 1) Pencere oluştur
    graf::GLWindow glwindow;
    if (glwindow.create(800, 600) < 0)
    {
        std::cerr << "Failed to create window.\n";
        return -1;
    }

    // 2) SceneMenu (GUI ile sahneyi yöneteceğimiz türemiş sınıf)
    graf::SceneMenu scene;

    // 3) Kamera0 (varsayılan)
    auto* camera0 = scene.getActiveCamera();
    camera0->getTransform()->setPosition(glm::vec3(0.f, 1.5f, 10.f));
    camera0->getTransform()->setEuler(glm::vec3(0.f, 0.f, 0.f)); 
    // Öne bakıyor (Z+)

    // 4) Kamera1 (ekstra kamera)
    auto* camera1 = new graf::Camera(60.f, 800.f / 600.f, 0.1f, 1000.f); // Yeni kamera
    camera1->getTransform()->setPosition(glm::vec3(0.f, 5.f, 10.f));
    camera1->getTransform()->setEuler(glm::vec3(-30.f, 0.f, 0.f));
    scene.addExtraCamera(camera1); // Yeni kamera sahneye ekleniyor

    // 5) TopViewCamera (mini viewport)
    auto* topViewCamera = new graf::Camera(60.f, 1.f, 0.1f, 1000.f);
    topViewCamera->getTransform()->setPosition({0.f, 20.f, 0.f});
    topViewCamera->getTransform()->setEuler({-90.f, 0.f, 0.f}); 
    // yukarıdan baksın

    // 6) Shader yükle
    graf::ShaderManager::addShaderFromFile("TextureShader");
    auto* prog = graf::ShaderManager::getProgram("TextureShader");
    if (prog)
    {
        prog->use();
        // “mySampler” => 0
        prog->setInt("mySampler", 0);
        prog->unuse();
    }

    // 7) Texture yükle
    graf::TextureManager::addTextureFromFile("grass_texture.jpg");
    graf::TextureManager::addTextureFromFile("hava.jpg");
    graf::TextureManager::addTextureFromFile("rock.jpg");
    graf::TextureManager::addTextureFromFile("stone_wall_texture.png");
    graf::TextureManager::addTextureFromFile("container.jpg");
    graf::TextureManager::addTextureFromFile("white_texture.png");
    graf::TextureManager::addTextureFromFile("wall.jpg");

    // 8) Zemin
    {
        auto ground = new graf::Model();
        ground->setVertexArrayObject(
            graf::ShapeCreator::createShape(graf::ShapeTypes::Square)
        );
        ground->setShapeType(graf::ShapeTypes::Square); 
        ground->setShaderProgramName("TextureShader");
        ground->setTextureName("grass_texture.jpg");
        // Yatay döndür (X ekseni -90°)
        ground->getTransform()->setEuler(glm::vec3(-90.f, 0.f, 0.f));
        // 15x15 büyüklük
        ground->getTransform()->setScale(glm::vec3(15.f, 15.f, 1.f));
        scene.addModel(ground);
    }

    // 9) Duvar oluşturma (ön, sağ, sol)
    auto createWall = [&](const glm::vec3& pos, const glm::vec3& eul, const glm::vec3& scl)
    {
        auto wall = new graf::Model();
        wall->setVertexArrayObject(
            graf::ShapeCreator::createShape(graf::ShapeTypes::Square)
        );
        wall->setShapeType(graf::ShapeTypes::Square);
        wall->setShaderProgramName("TextureShader");
        wall->setTextureName("wall.jpg");

        wall->getTransform()->setPosition(pos);
        wall->getTransform()->setEuler(eul);
        wall->getTransform()->setScale(scl);

        scene.addModel(wall);
    };

    // Duvarlar
    createWall(glm::vec3(0.f, 1.5f, -7.5), glm::vec3(0.f, 0.f, 0.f), glm::vec3(15.f, 3.f, 1.f));
    createWall(glm::vec3(7.5f, 1.5f, 0.f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(15.f, 3.f, 1.f));
    createWall(glm::vec3(-7.5f, 1.5f, 0.f), glm::vec3(0.f, -90.f, 0.f), glm::vec3(15.f, 3.f, 1.f));

    // 10) Ortada küp
    {
        auto cube = new graf::Model();
        cube->setVertexArrayObject(
            graf::ShapeCreator::createShape(graf::ShapeTypes::Cube)
        );
        cube->setShapeType(graf::ShapeTypes::Cube);
        cube->setShaderProgramName("TextureShader");
        cube->setTextureName("container.jpg");
        cube->getTransform()->setPosition(glm::vec3(0.f, 1.f, 0.f));
        scene.addModel(cube);
    }

    // **Manuel olarak eklenen kamera göstergelerini kaldırın**
    // Artık `Scene` sınıfı bu göstergeleri otomatik olarak ekleyecek

    // 12) Render döngüsü
    glwindow.setRenderFunction([&](){
        int w,h;
        glfwGetFramebufferSize(glwindow.getWindow(), &w, &h);
        glViewport(0,0,w,h);
        glClearColor(0.f,0.4f,0.7f,1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Ana sahneyi çiz
        scene.render(true); // => GUI var

        // Sağ üst mini viewport => topViewCamera
        int miniW = w/4, miniH = h/4;
        int miniX = w - miniW - 10;
        int miniY = h - miniH - 10;
        glViewport(miniX, miniY, miniW, miniH);
        auto oldCam = scene.getActiveCamera();    // kaydet
        scene.setActiveCamera(topViewCamera);
        scene.render(false);                           // küçük pencerede
        scene.setActiveCamera(oldCam);            // geri al
    });

    // 13) Başlat
    glwindow.render();

    return 0;
}
