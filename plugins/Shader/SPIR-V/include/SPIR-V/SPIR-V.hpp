///
/// @file SPIR-V.hpp
/// @brief This file contains the SPIR-V class declaration
/// @namespace cae
///

#pragma once

#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>

#include "Interfaces/Renderer/IShader.hpp"

#include <unordered_map>
#include <stdexcept>

namespace cae
{

    constexpr auto VERSION = 450;

    ///
    /// @class SPIRV
    /// @brief Class for the SPIRV plugin
    /// @namespace cae
    ///
    class SPIRV final : public IShader
    {

        public:
            SPIRV() = default;
            ~SPIRV() override = default;

            SPIRV(const SPIRV &) = delete;
            SPIRV &operator=(const SPIRV &) = delete;
            SPIRV(SPIRV &&) = delete;
            SPIRV &operator=(SPIRV &&) = delete;

            [[nodiscard]] std::string getName() const override { return "SPIRV"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::SHADER; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::LINUX; }


            void addShader(const ShaderModuleDesc& pipelineDesc) override;
            bool compileAll() override;
            const ShaderData& getShader(const std::string& name) const override;
            bool isCompiled(const std::string& name) const override;

        private:
            std::unordered_map<std::string, ShaderData> m_shaders;
            std::unordered_map<std::string, bool> m_compiled;

            static EShLanguage shaderStageToESh(const ShaderStage stage) {
                switch (stage) {
                    case ShaderStage::VERTEX: return EShLangVertex;
                    case ShaderStage::FRAGMENT: return EShLangFragment;
                    case ShaderStage::GEOMETRY: return EShLangGeometry;
                    case ShaderStage::COMPUTE: return EShLangCompute;
                    default: throw std::runtime_error("Unsupported ShaderStage");
                }
            }

            static std::vector<uint32_t> compileGLSLtoSPIRV(const std::string& src, const ShaderStage stage) {
                static bool glslangInitialized = false;
                if (!glslangInitialized) {
                    glslang::InitializeProcess();
                    glslangInitialized = true;
                }

                const EShLanguage lang = shaderStageToESh(stage);
                glslang::TShader shader(lang);
                const char* shaderStrings[1] = { src.c_str() };
                shader.setStrings(shaderStrings, 1);

                shader.setEnvInput(glslang::EShSourceGlsl, lang, glslang::EShClientVulkan, VERSION);
                shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_3);
                shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_6);

                TBuiltInResource Resources = {};
                Resources.maxLights = 32;
                Resources.maxClipPlanes = 6;
                Resources.maxTextureUnits = 32;
                Resources.maxTextureCoords = 32;
                Resources.maxVertexAttribs = 64;
                Resources.maxVertexUniformComponents = 4096;
                Resources.maxVaryingFloats = 64;
                Resources.maxVertexTextureImageUnits = 32;
                Resources.maxCombinedTextureImageUnits = 80;
                Resources.maxTextureImageUnits = 32;
                Resources.maxFragmentUniformComponents = 4096;
                Resources.maxDrawBuffers = 32;
                Resources.maxVertexUniformVectors = 128;
                Resources.maxVaryingVectors = 8;
                Resources.maxFragmentUniformVectors = 16;
                Resources.maxVertexOutputVectors = 16;
                Resources.maxFragmentInputVectors = 15;
                Resources.minProgramTexelOffset = -8;
                Resources.maxProgramTexelOffset = 7;
                Resources.maxClipDistances = 8;
                Resources.maxComputeWorkGroupCountX = 65535;
                Resources.maxComputeWorkGroupCountY = 65535;
                Resources.maxComputeWorkGroupCountZ = 65535;
                Resources.maxComputeWorkGroupSizeX = 1024;
                Resources.maxComputeWorkGroupSizeY = 1024;
                Resources.maxComputeWorkGroupSizeZ = 64;
                Resources.maxComputeUniformComponents = 1024;
                Resources.maxComputeTextureImageUnits = 16;
                Resources.maxComputeImageUniforms = 8;
                Resources.maxComputeAtomicCounters = 8;
                Resources.maxComputeAtomicCounterBuffers = 1;
                Resources.maxVaryingComponents = 60;
                Resources.maxVertexOutputComponents = 64;
                Resources.maxGeometryInputComponents = 64;
                Resources.maxGeometryOutputComponents = 128;
                Resources.maxFragmentInputComponents = 128;
                Resources.maxImageUnits = 8;
                Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
                Resources.maxCombinedShaderOutputResources = 8;

                constexpr auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

                if (!shader.parse(&Resources, VERSION, false, messages)) {
                    throw std::runtime_error("GLSL parsing failed: " + std::string(shader.getInfoLog()));
                }

                glslang::TProgram program;
                program.addShader(&shader);

                if (!program.link(messages)) {
                    throw std::runtime_error("GLSL linking failed: " + std::string(program.getInfoLog()));
                }

                std::vector<uint32_t> spirv;
                glslang::GlslangToSpv(*program.getIntermediate(lang), spirv);

                return spirv;
            }

    }; // class SPIRV

} // namespace cae
