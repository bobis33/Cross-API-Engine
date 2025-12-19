///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace cae
///

#pragma once

#include "Interfaces/IWindow.hpp"

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

            virtual void initialize(const NativeWindowHandle &nativeWindowHandle, const Color& clearColor = { 1.F, 1.F, 1.F, 1.F}) = 0;
            virtual void draw(const WindowSize &windowSize) = 0;

            virtual void setVSyncEnabled(bool enabled) = 0;
            virtual bool isVSyncEnabled() const = 0;

            virtual void setClearColor(const Color& color) = 0;

    }; // interface IRenderer

} // namespace cae
