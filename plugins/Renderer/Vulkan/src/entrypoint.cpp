#include "VULKN/VULKN.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IRenderer *entryPoint() { return std::make_unique<cae::VULKN>().release(); }
}