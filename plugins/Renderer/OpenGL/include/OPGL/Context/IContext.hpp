///
/// @file IContext.hpp
/// @brief This file contains the IContext interface
/// @namespace cae
///

#pragma once

#include "Interfaces/IWindow.hpp"

#include <glad/gl.h>

namespace cae
{
    ///
    /// @interface IContext
    /// @brief Interface for OpenGL context
    /// @namespace cae
    ///
    class IContext
    {
        public:
            virtual ~IContext() = default;

            ///
            /// @param window The native window handle
            /// @brief Initialize the OpenGL context with the given window
            ///
            virtual void initialize(const NativeWindowHandle &window) = 0;

            ///
            /// @brief Swap the front and back buffers
            ///
            virtual void swapBuffers() = 0;

            ///
            /// @param enabled Whether VSync should be enabled
            /// @brief Enable or disable VSync
            ///
            virtual void setVSyncEnabled(bool enabled) = 0;

            ///
            /// @return  Whether VSync is enabled
            /// @brief Check if VSync is enabled
            ///
            [[nodiscard]] virtual bool isVSyncEnabled() const = 0;

            GladGLContext gl{nullptr};

    }; // interface IContext

} // namespace cae
