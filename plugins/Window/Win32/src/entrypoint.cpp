#include "Win32/Win32.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IWindow *entryPoint() { return std::make_unique<cae::Win32>().release(); }
}
