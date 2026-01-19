///
/// @file IShaderIR.hpp
/// @brief This file contains the ShaderIR interface
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/Frontend/IShaderFrontend.hpp"

#include <span>
#include <stdexcept>

namespace cae
{

    ///
    /// @interface IShaderIR
    /// @brief Interface for shaders IR
    /// @namespace cae
    ///
    class IShaderIR : public utl::IPlugin
    {

        public:
            ~IShaderIR() override = default;

            ///
            /// @return The IR type this processor handles
            /// @brief Get the IR type this processor handles
            ///
            virtual ShaderSourceType irType() const = 0;

            ///
            /// @brief Transform or validate a shader IR module
            /// @param module The input IR module
            /// @return Transformed IR module ready for the backend
            ///
            virtual ShaderIRModule process(const ShaderIRModule &module) = 0;

            ///
            /// @brief Optional: optimize a batch of IR modules
            ///
            virtual void optimize(std::span<ShaderIRModule> modules) { /* default: no-op */ }

            ///
            /// @brief Optional: cross-compile from one IR to another (SPIR-V -> MSL, etc.)
            ///
            virtual ShaderIRModule crossCompile(const ShaderIRModule &module, ShaderSourceType targetType)
            {
                throw std::runtime_error("Cross-compilation not implemented");
            }

    }; // interface IShaderIR

} // namespace cae
