#include "SPIRV/SPIRV.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IShaderIR *entryPoint() { return std::make_unique<cae::SPIRV>().release(); }
}
