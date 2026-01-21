///
/// @file AShaderIR.hpp
/// @brief This file contains the ShaderIR abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/IR/IShaderIR.hpp"

namespace cae
{

    ///
    /// @interface AShaderIR
    /// @brief Abstract class for shader IR
    /// @namespace cae
    ///
    class AShaderIR : public IShaderIR
    {

        public:
            ~AShaderIR() override = default;

    }; // interface AShaderIR

} // namespace cae
