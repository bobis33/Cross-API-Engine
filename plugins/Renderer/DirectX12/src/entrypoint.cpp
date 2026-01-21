#include "DirectX12/DirectX12.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IRenderer *entryPoint() { return std::make_unique<cae::DirectX12>().release(); }
}