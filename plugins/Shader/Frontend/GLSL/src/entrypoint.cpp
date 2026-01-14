#include "GLSL/GLSL.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IShader *entryPoint() { return std::make_unique<cae::GLSL>().release(); }
}
