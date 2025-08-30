#include <memory>

#include "OPGL/OPGL.hpp"

extern "C"
{
    std::unique_ptr<cae::IRenderer> entryPoint()
    {
        return std::make_unique<cae::OPGL>();
    }
}