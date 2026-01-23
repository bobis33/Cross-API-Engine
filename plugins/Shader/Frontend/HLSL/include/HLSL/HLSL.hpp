///
/// @file HLSL.hpp
/// @brief This file contains the HLSL class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/Frontend/AShaderFrontend.hpp"

#include <stdexcept>

namespace cae
{

    constexpr auto VERSION = 450;

    ///
    /// @class HLSL
    /// @brief Class for the HLSL plugin
    /// @namespace cae
    ///
    class HLSL final : public AShaderFrontend
    {

        public:
            HLSL() = default;
            ~HLSL() override = default;

            HLSL(const HLSL &) = delete;
            HLSL &operator=(const HLSL &) = delete;
            HLSL(HLSL &&) = delete;
            HLSL &operator=(HLSL &&) = delete;

            [[nodiscard]] std::string getName() const override { return "HLSL"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::SHADER_FRONTEND; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

            [[nodiscard]] ShaderSourceType sourceType() const override { return ShaderSourceType::HLSL; }

            ShaderIRModule compile(const ShaderSourceDesc &desc) override;

        private:
            static std::string stageToTargetProfile(ShaderStage stage, bool dx12);

    }; // class HLSL

} // namespace cae
