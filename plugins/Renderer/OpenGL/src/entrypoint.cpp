#include "OPGL/OPGL.hpp"

extern "C"
{
    PLUGIN_EXPORT cae::IRenderer *entryPoint() { return std::make_unique<cae::OPGL>().release(); }
}