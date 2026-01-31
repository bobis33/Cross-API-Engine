///
/// @file SPIRV.hpp
/// @brief This file contains the SpirvIR class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/AShaderCompiler.hpp"
#include "Utils/Interfaces/IPlugin.hpp"

#include <span>
#include <stdexcept>
#include <string>

namespace cae
{

    ///
    /// @class SPIRV
    /// @brief Class for the SPIR-V IR plugin
    /// @namespace cae
    ///
    class SPIRV final : public AShaderCompiler
    {
        public:
            SPIRV() = default;
            ~SPIRV() override = default;

            SPIRV(const SPIRV &) = delete;
            SPIRV &operator=(const SPIRV &) = delete;
            SPIRV(SPIRV &&) = delete;
            SPIRV &operator=(SPIRV &&) = delete;

            [[nodiscard]] std::string getName() const override { return "SPIRV"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::SHADER_IR; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

            // [[nodiscard]] ShaderSourceType irType() const override { return ShaderSourceType::SPIRV; }

            // ShaderIRModule process(const ShaderIRModule &module) override
            // {
            //     ShaderIRModule out = module;
            //     return out;
            // }

            // void optimize(std::span<ShaderIRModule> modules) override {}

            // ShaderIRModule crossCompile(const ShaderIRModule &module, const ShaderSourceType targetType) override
            // {
            //     if (targetType == ShaderSourceType::MSL)
            //     {
            //         ShaderIRModule out = module;
            //         return out;
            //     }

               //  throw std::runtime_error("Cross-compilation to this target not implemented");
            // }

            ShaderIRModule compile(const ShaderSourceDesc &desc) override
            {

            }


    }; // class SPIRV

} // namespace cae
