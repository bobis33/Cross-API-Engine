#include <memory>

#include "CAE/Interfaces/IRenderer.hpp"
#include "OPGL/OPGL.hpp"

extern "C"
{
    std::unique_ptr<CAE::IRenderer> entryPoint()
    {
        return std::make_unique<CAE::OPGL>();
    }
}