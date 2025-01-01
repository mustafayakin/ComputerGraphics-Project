#include "glwindow.hpp"
#include <iostream>

// ImGui ile ilgili
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace graf {

void GLWindow::statickeyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLWindow* myWindow = (GLWindow*)glfwGetWindowUserPointer(window);
    if (myWindow && myWindow->m_keyboardFunction) {
        myWindow->m_keyboardFunction(key, scancode, action);
    }
}

void GLWindow::staticMouseFunction(GLFWwindow* window, double xpos, double ypos)
{
    GLWindow* myWindow = (GLWindow*)glfwGetWindowUserPointer(window);
    if (myWindow && myWindow->m_mouseFunction) {
        myWindow->m_mouseFunction(xpos, ypos);
    }
}

void GLWindow::setMouseFunction(MouseFunction function)
{
    m_mouseFunction = function;
}

void GLWindow::setKeyboardFunction(KeyboardFunction function)
{
    m_keyboardFunction = function;
}

int GLWindow::create(unsigned int width,unsigned int height)
{
    if(!glfwInit())
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pencere oluştur
    m_window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_window);

    // GLAD yükle
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // GLFW callback’leri
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, statickeyboardFunction);
    glfwSetCursorPosCallback(m_window, staticMouseFunction);

    // Texture wrap (sizin kodlarınız)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // ImGui kur
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return 1;
}

void GLWindow::setRenderFunction(RenderFunction render)
{
    m_renderFunction = render;
}

void GLWindow::render()
{
    while (!glfwWindowShouldClose(m_window))
    {
        // ImGui frame başlat
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Kullanıcının tanımladığı render fonksiyonu
        if (m_renderFunction) {
            m_renderFunction();
        }

        // ImGui çizimi
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

}
