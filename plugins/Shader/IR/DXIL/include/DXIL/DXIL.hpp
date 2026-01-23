///
/// @file DXIL.hpp
/// @brief This file contains the DXIL class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/IR/AShaderIR.hpp"

namespace cae
{

    ///
    /// @class DXIL
    /// @brief Class for the DXIL IR plugin
    /// @namespace cae
    ///
    class DXIL final : public AShaderIR
    {
        public:
            DXIL() = default;
            ~DXIL() override = default;

            DXIL(const DXIL &) = delete;
            DXIL &operator=(const DXIL &) = delete;
            DXIL(DXIL &&) = delete;
            DXIL &operator=(DXIL &&) = delete;

            [[nodiscard]] std::string getName() const override { return "DXIL"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::SHADER_IR; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

            [[nodiscard]] ShaderSourceType irType() const override { return ShaderSourceType::DXIL; }

            ShaderIRModule process(const ShaderIRModule &module) override
            {
                ShaderIRModule out = module;
                return out;
            }

            void optimize(std::span<ShaderIRModule> modules) override {}

            ShaderIRModule crossCompile(const ShaderIRModule &module, const ShaderSourceType targetType) override
            {
                return module;
            }

    }; // class DXIL

} // namespace cae
