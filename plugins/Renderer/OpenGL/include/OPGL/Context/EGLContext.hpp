///
/// @file EGLContext.hpp
/// @brief This file contains the EGLContext_ class declaration
/// @namespace cae
///

#pragma once

#ifdef __linux__

#include "OPGL/Context/IContext.hpp"

#include <EGL/egl.h>

namespace cae
{

    ///
    /// @class EGLContext_
    /// @brief Implementation of IContext for Linux using EGL
    /// @namespace cae
    ///
    class EGLContext_ final : public IContext
    {
        public:
            explicit EGLContext_() = default;
            ~EGLContext_() override;

            void initialize(const NativeWindowHandle &window) override;

            void swapBuffers() override;

            void setVSyncEnabled(bool enabled) override;
            [[nodiscard]] bool isVSyncEnabled() const override
            {
                // EGL does not provide a direct way to query VSync status
                return false;
            }

        private:
            EGLDisplay m_display = EGL_NO_DISPLAY;
            EGLSurface m_surface = EGL_NO_SURFACE;
            EGLContext m_context = EGL_NO_CONTEXT;

    }; // class EGLContext_

} // namespace cae

#endif
