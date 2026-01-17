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

            virtual void initialize(const NativeWindowHandle &window) = 0;

            virtual void swapBuffers() = 0;

            virtual void setVSyncEnabled(bool enabled) = 0;
            [[nodiscard]] virtual bool isVSyncEnabled() const = 0;

            GladGLContext gl{0};

    }; // interface IContext

} // namespace cae
