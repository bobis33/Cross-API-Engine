#include "HLSL/HLSL.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IShaderFrontend *entryPoint() { return std::make_unique<cae::HLSL>().release(); }
}
