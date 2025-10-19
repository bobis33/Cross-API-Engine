#include <memory>

#include "VULKN/VULKN.hpp"

extern "C"
{
    PLUGIN_EXPORT cae::IRenderer *entryPoint() { return std::make_unique<cae::VULKN>().release(); }
}