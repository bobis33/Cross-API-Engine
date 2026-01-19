#include "Cocoa/Cocoa.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IWindow *entryPoint() { return std::make_unique<cae::Cocoa>().release(); }
}
