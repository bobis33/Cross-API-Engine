#include <memory>

#include "Interfaces/IWindow.hpp"
#include "X11/X11.hpp"

extern "C"
{
    PLUGIN_EXPORT cae::IWindow *entryPoint() { return std::make_unique<cae::X11>().release(); }
}
