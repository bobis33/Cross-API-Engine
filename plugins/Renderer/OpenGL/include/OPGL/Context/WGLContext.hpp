///
/// @file WGLContext.hpp
/// @brief This file contains the WGLContext class declaration
/// @namespace cae
///

#pragma once

#ifdef _WIN32

#include "OPGL/Context/IContext.hpp"

#include <Windows.h>

namespace cae
{

    ///
    /// @class WGLContext
    /// @brief Implementation of IContext for Windows using WGL
    /// @namespace cae
    ///
    class WGLContext final : public IContext
    {
        public:
            WGLContext() = default;
            ~WGLContext() override;

            void initialize(const NativeWindowHandle &window) override;
            void swapBuffers() override;
            void setVSyncEnabled(bool enabled) override;
            [[nodiscard]] bool isVSyncEnabled() const override
            {
                // WGL does not provide a direct way to query VSync status
                return false;
            }

        private:
            HWND m_hwnd = nullptr;
            HDC m_hdc = nullptr;
            HGLRC m_hglrc = nullptr;

    }; // class WGLContext

} // namespace cae

#endif
