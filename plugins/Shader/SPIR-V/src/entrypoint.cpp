#include <memory>

#include "SPIR-V/SPIR-V.hpp"

extern "C"
{
    PLUGIN_EXPORT cae::IShader *entryPoint() { return std::make_unique<cae::SPIRV>().release(); }
}
