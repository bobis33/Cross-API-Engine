#include <memory>

#include "GLFW/GLFW.hpp"

extern "C"
{
    PLUGIN_EXPORT cae::IWindow *entryPoint() { return std::make_unique<cae::GLFW>().release(); }
}
