///
/// @file WGLContextWindows.hpp
/// @brief This file contains the WGLContextWindows class declaration
/// @namespace cae
///

#pragma once

#if defined(_WIN32)
#include <Windows.h>

#include "IContext.hpp"

namespace cae
{

    ///
    /// @class WGLContextWindows
    /// @brief Implementation of IContext for Windows using WGL
    /// @namespace cae
    ///
    class WGLContextWindows final : public IContext
    {
        public:
            WGLContextWindows() = default;
            ~WGLContextWindows() override;

            void initialize(const NativeWindowHandle &window) override;
            void swapBuffers() override;
            void setVSyncEnabled(const bool enabled) override;
            [[nodiscard]] bool isVSyncEnabled() const override
            {
                // WGL does not provide a direct way to query VSync status
                return false;
            }

        private:
            HWND m_hwnd = nullptr;
            HDC m_hdc = nullptr;
            HGLRC m_hglrc = nullptr;

    }; // class WGLContextWindows

} // namespace cae

#endif
