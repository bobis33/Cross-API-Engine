#include "WebGPU/WebGPU.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IRenderer *entryPoint() { return std::make_unique<cae::WebGPU>().release(); }
}