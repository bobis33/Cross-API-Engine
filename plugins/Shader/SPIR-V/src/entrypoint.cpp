#include "SPIR-V/SPIR-V.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IShader *entryPoint() { return std::make_unique<cae::SPIRV>().release(); }
}
