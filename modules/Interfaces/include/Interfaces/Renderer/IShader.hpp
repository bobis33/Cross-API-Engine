///
/// @file IShader.hpp
/// @brief This file contains the Shader interface
/// @namespace cae
///

#pragma once

#include "Utils/Interfaces/IPlugin.hpp"

namespace cae
{

    ///
    /// @interface IShader
    /// @brief Interface for shaders
    /// @namespace cae
    ///
    class IShader : public utl::IPlugin
    {

        public:
            ~IShader() override = default;

    }; // interface IShader

} // namespace cae
