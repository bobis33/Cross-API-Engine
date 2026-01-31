///
/// @file IShaderCompiler.hpp
/// @brief This file contains the ShaderCompiler abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/IShaderCompiler.hpp"

namespace cae
{

    ///
    /// @interface AShaderCompiler
    /// @brief Abstract class for shader compiler
    /// @namespace cae
    ///
    class AShaderCompiler : public IShaderCompiler
    {

        public:
            ~AShaderCompiler() override = default;

    }; // interface AShaderCompiler

} // namespace cae
