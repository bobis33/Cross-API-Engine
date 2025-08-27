#include <memory>

#include "CAE/Interfaces/IRenderer.hpp"
#include "VULKN/VULKN.hpp"

extern "C"
{
    std::unique_ptr<CAE::IRenderer> entryPoint()
    {
        return std::make_unique<CAE::VULKN>();
    }
}