#include "DirectX11/DirectX11.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IRenderer *entryPoint() { return std::make_unique<cae::DirectX11>().release(); }
}