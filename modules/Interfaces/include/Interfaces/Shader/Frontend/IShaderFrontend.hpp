///
/// @file IShaderFrontend.hpp
/// @brief This file contains the ShaderFrontend interface
/// @namespace cae
///

#pragma once

#include "Utils/Interfaces/IPlugin.hpp"

#include <vector>

namespace cae
{

    using ShaderID = std::string;

    enum class ShaderSourceType : uint8_t
    {
        GLSL = 0,
        HLSL = 1,
        WGSL = 2,
        MSL = 3,
        SPIRV = 4,
        DXIL = 5,
        DXBC = 6,
        UNDEFINED = 255
    };

    enum class ShaderStage : uint8_t
    {
        VERTEX = 0,
        FRAGMENT = 1,
        GEOMETRY = 2,
        COMPUTE = 3,
        UNDEFINED = 255
    };

    ///
    /// @struct ShaderSourceDesc
    /// @brief Struct for shader source description
    /// @namespace cae
    ///
    struct ShaderSourceDesc
    {
            ShaderID id;
            ShaderSourceType type;
            std::string source;
            ShaderStage stage;
    };

    ///
    /// @struct ShaderIRModule
    /// @brief Struct for shader intermediate representation module
    /// @namespace cae
    ///
    struct ShaderIRModule
    {
            ShaderID id;
            ShaderStage stage;
            // std::vector<uint8_t> dxbc;
            // std::vector<uint8_t> dxil;
            std::vector<uint32_t> spirv;
            std::string entryPoint = "main";
    };

    ///
    /// @struct ShaderPipelineDesc
    /// @brief Struct for shader pipeline description
    /// @namespace cae
    ///
    struct ShaderPipelineDesc
    {
            ShaderID id;
            ShaderID vertex;
            ShaderID fragment;
    };

    ///
    /// @interface IShaderFrontend
    /// @brief Interface for shaders frontend
    /// @namespace cae
    ///
    class IShaderFrontend : public utl::IPlugin
    {

        public:
            ~IShaderFrontend() override = default;

            ///
            /// @return The source type this frontend handles
            /// @brief Get the source type this frontend handles
            ///
            virtual ShaderSourceType sourceType() const = 0;

            ///
            /// @param desc Shader source description
            /// @return Compiled shader intermediate representation module
            /// @brief Compile shader source to intermediate representation
            ///
            virtual ShaderIRModule compile(const ShaderSourceDesc &desc) = 0;

    }; // interface IShaderFrontend

} // namespace cae
