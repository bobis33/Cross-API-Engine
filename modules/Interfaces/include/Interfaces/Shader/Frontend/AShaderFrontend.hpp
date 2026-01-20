///
/// @file AShaderFrontend.hpp
/// @brief This file contains the ShaderFrontend abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/Frontend/IShaderFrontend.hpp"

namespace cae
{

    ///
    /// @interface AShaderFrontend
    /// @brief Abstract class for shader frontend
    /// @namespace cae
    ///
    class AShaderFrontend : public IShaderFrontend
    {

        public:
            ~AShaderFrontend() override = default;

    }; // interface AShaderFrontend

} // namespace cae
