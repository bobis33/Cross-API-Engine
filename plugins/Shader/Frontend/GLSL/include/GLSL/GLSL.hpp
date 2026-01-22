///
/// @file GLSL.hpp
/// @brief This file contains the GLSL class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/Frontend/AShaderFrontend.hpp"

#include <glslang/Public/ShaderLang.h>

#include <stdexcept>

namespace cae
{

    constexpr auto VERSION = 450;

    ///
    /// @class GLSL
    /// @brief Class for the GLSL plugin
    /// @namespace cae
    ///
    class GLSL final : public AShaderFrontend
    {

        public:
            GLSL() = default;
            ~GLSL() override = default;

            GLSL(const GLSL &) = delete;
            GLSL &operator=(const GLSL &) = delete;
            GLSL(GLSL &&) = delete;
            GLSL &operator=(GLSL &&) = delete;

            [[nodiscard]] std::string getName() const override { return "GLSL"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::SHADER_FRONTEND; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

            [[nodiscard]] ShaderSourceType sourceType() const override { return ShaderSourceType::GLSL; }

            ShaderIRModule compile(const ShaderSourceDesc &desc) override
            {
                ShaderIRModule ir;
                ir.id = desc.id;
                ir.stage = desc.stage;
                ir.entryPoint = "main";
                ir.spirv = compileGLSLtoSPIRV(desc.source, desc.stage);
                return ir;
            }

        private:
            static EShLanguage shaderStageToESh(const ShaderStage stage)
            {
                switch (stage)
                {
                    case ShaderStage::VERTEX:
                        return EShLangVertex;
                    case ShaderStage::FRAGMENT:
                        return EShLangFragment;
                    case ShaderStage::GEOMETRY:
                        return EShLangGeometry;
                    case ShaderStage::COMPUTE:
                        return EShLangCompute;
                    default:
                        throw std::runtime_error("Unsupported ShaderStage");
                }
            }

            static std::vector<std::uint32_t> compileGLSLtoSPIRV(const std::string &src, ShaderStage stage);

    }; // class GLSL

} // namespace cae
