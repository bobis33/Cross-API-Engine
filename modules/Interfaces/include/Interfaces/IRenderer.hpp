///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace cae
///

#pragma once

#include "Interfaces/IWindow.hpp"

namespace cae
{

    ///
    /// @interface IRenderer
    /// @brief Interface for renderer
    /// @namespace cae
    ///
    class IRenderer : public utl::IPlugin
    {

        public:
            ~IRenderer() override = default;

            virtual void initialize(const NativeWindowHandle &nativeWindowHandle) = 0;
            virtual void draw(const WindowSize &windowSize) = 0;
    }; // interface IRenderer

} // namespace cae
