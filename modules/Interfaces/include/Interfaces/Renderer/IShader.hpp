///
/// @file IShader.hpp
/// @brief This file contains the Shader interface
/// @namespace cae
///

#pragma once

#include <vector>

#include "Utils/Interfaces/IPlugin.hpp"

enum class ShaderSourceType : uint8_t
{
    GLSL,
    HLSL,
    SPIRV,
    MSL,
    WGSL,
    UNDEFINED = 255
};

enum class ShaderStage : uint8_t
{
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    COMPUTE,
    UNDEFINED = 255
};

struct ShaderData {
    ShaderSourceType type;
    std::string source;              // GLSL/HLSL/etc. pour debug/fallback
    std::vector<uint32_t> spirv;     // SPIR-V compilé
    ShaderStage stage;
    std::string entryPoint = "main";
};

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

            virtual void addShader(const std::string& name, const std::string& source, ShaderStage stage) = 0;
            virtual bool compileAll() = 0;
            virtual const ShaderData& getShader(const std::string& name) const = 0;
            virtual bool isCompiled(const std::string& name) const = 0;

    }; // interface IShader

} // namespace cae
