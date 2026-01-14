#include "X11/X11.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IWindow *entryPoint() { return std::make_unique<cae::X11>().release(); }
}
