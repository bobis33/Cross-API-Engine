#include <memory>

#include "VULKN/VULKN.hpp"

extern "C"
{
    cae::IRenderer *entryPoint()
    {
        return std::make_unique<cae::VULKN>().release();
    }
}