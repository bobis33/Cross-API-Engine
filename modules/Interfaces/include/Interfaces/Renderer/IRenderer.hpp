///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace cae
///

#pragma once

#include "Interfaces/IWindow.hpp"
#include "Interfaces/Shader/IShaderFrontend.hpp"

namespace cae
{

    struct Color
    {
            float r;
            float g;
            float b;
            float a;
    };

    ///
    /// @interface IRenderer
    /// @brief Interface for renderer
    /// @namespace cae
    ///
    class IRenderer : public utl::IPlugin
    {

        public:
            ~IRenderer() override = default;

            virtual void setVSyncEnabled(bool enabled) = 0;
            virtual void setClearColor(const Color &color) = 0;

            [[nodiscard]] virtual bool isVSyncEnabled() const = 0;

            virtual void initialize(const NativeWindowHandle &nativeWindowHandle,
                                    const Color &clearColor = {.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F}) = 0;
            virtual void createPipeline(const ShaderID &id, const ShaderIRModule &vertex,
                                        const ShaderIRModule &fragment) = 0;
            virtual void draw(const WindowSize &windowSize, const ShaderID &shaderId) = 0;
            virtual void createMesh(const std::vector<float> &vertices) = 0;
    }; // interface IRenderer

} // namespace cae
