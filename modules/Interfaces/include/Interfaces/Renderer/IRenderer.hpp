///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace cae
///

#pragma once

#include "Interfaces/IWindow.hpp"
#include "Interfaces/Shader/IShaderFrontend.hpp"

#include <glm/mat4x4.hpp>

namespace cae
{

    ///
    /// @struct Color
    /// @brief Struct for color
    /// @namespace cae
    ///
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

            ///
            /// @param enabled Whether VSync is enabled
            /// @brief Enable or disable VSync
            ///
            virtual void setVSyncEnabled(bool enabled) = 0;

            ///
            /// @param color Clear color to set
            /// @brief Set the clear color
            ///
            virtual void setClearColor(const Color &color) = 0;

            ///
            /// @return Whether VSync is enabled
            /// @brief Check if VSync is enabled
            ///
            [[nodiscard]] virtual bool isVSyncEnabled() const = 0;

            ///
            /// @param nativeWindowHandle Native window handle
            /// @param clearColor Clear color (default: white)
            /// @brief Initialize the renderer with a native window handle and clear color
            ///
            virtual void initialize(const NativeWindowHandle &nativeWindowHandle,
                                    const Color &clearColor = {.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F}) = 0;

            ///
            /// @param id Shader ID
            /// @param vertex Vertex shader IR module
            /// @param fragment Fragment shader IR module
            /// @brief Create a rendering pipeline with vertex and fragment shaders
            ///
            virtual void createPipeline(const ShaderID &id, const ShaderIRModule &vertex,
                                        const ShaderIRModule &fragment) = 0;

            ///
            /// @param windowSize Current window size
            /// @param shaderId Shader ID to use for drawing
            /// @param mvp Model-View-Projection matrix
            /// @brief Draw the scene using the specified shader and window size
            ///
            virtual void draw(const WindowSize &windowSize, const ShaderID &shaderId, glm::mat4 mvp) = 0;

            ///
            /// @param vertices Vertex data to create the mesh
            /// @brief Create a mesh with the given vertex data
            ///
            virtual void createMesh(const std::vector<float> &vertices) = 0;

    }; // interface IRenderer

} // namespace cae
