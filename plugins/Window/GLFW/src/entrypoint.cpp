#include <memory>

#include "GLFW/GLFW.hpp"
#include "Interfaces/IWindow.hpp"

extern "C"
{
    cae::IWindow *entryPoint() { return std::make_unique<cae::GLFW>().release(); }
}
