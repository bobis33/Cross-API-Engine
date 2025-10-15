#include <memory>

#include "X11/X11.hpp"
#include "Interfaces/IWindow.hpp"

extern "C"
{
    cae::IWindow *entryPoint() { return std::make_unique<cae::X11>().release(); }
}
