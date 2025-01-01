#ifndef GLWINDOW_HPP
#define GLWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "functiontypes.hpp"

namespace graf {

    class GLWindow
    {
    public:
        // Pencere oluşturur
        int create(unsigned int width, unsigned int height);

        // Render döngüsünü çalıştırır
        void render();

        // Callback fonksiyonlarını atar
        void setRenderFunction(RenderFunction function);
        void setKeyboardFunction(KeyboardFunction function);
        void setMouseFunction(MouseFunction function);

        // GLFWwindow* işaretçisini dışarı veren ufak getWindow() metodu
        GLFWwindow* getWindow() const { return m_window; }

    private:
        GLFWwindow* m_window;

        static void staticMouseFunction(GLFWwindow* window, double xpos, double ypos);
        static void statickeyboardFunction(GLFWwindow* window, int key, int scancode, int action, int mods);

        RenderFunction    m_renderFunction;
        KeyboardFunction  m_keyboardFunction;
        MouseFunction     m_mouseFunction;
    };

}

#endif // GLWINDOW_HPP
