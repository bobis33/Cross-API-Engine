#include <iostream>
#include <memory>

#include "OPGL/OPGL.hpp"

extern "C"
{
    cae::IRenderer *entryPoint() { return std::make_unique<cae::OPGL>().release(); }
}