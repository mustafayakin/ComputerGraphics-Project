#ifndef FUNCTIONTYPES_HPP
#define FUNCTIONTYPES_HPP

#include <functional>

namespace graf {

    // Klavye callback: (key, scancode, action)
    using KeyboardFunction = std::function<void(int,int,int)>;

    // Fare callback: (xpos, ypos)
    using MouseFunction = std::function<void(double,double)>;

    // Render callback: her framedeki ana fonksiyon
    using RenderFunction = std::function<void(void)>;

}

#endif // FUNCTIONTYPES_HPP
