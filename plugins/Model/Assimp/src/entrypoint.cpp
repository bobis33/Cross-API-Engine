#include "Assimp/Assimp.hpp"

#include <memory>

extern "C"
{
    PLUGIN_EXPORT cae::IModel *entryPoint() { return std::make_unique<cae::Assimp>().release(); }
}