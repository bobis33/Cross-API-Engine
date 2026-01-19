///
/// @file Win32.hpp
/// @brief This file contains the Win32 class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Window/AWindow.hpp"

#include <windows.h>

#include <queue>

namespace cae
{

    ///
    /// @class Win32
    /// @brief Class for the Win32 plugin
    /// @namespace cae
    ///
    class Win32 final : public AWindow
    {

        public:
            Win32() = default;
            ~Win32() override = default;

            Win32(const Win32 &) = delete;
            Win32 &operator=(const Win32 &) = delete;
            Win32(Win32 &&) = delete;
            Win32 &operator=(Win32 &&) = delete;

            [[nodiscard]] std::string getName() const override { return "Win32"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::WINDOW; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::WINDOWS; }

            bool create(const std::string &name, WindowSize size) override;
            void close() override;

            [[nodiscard]] NativeWindowHandle getNativeHandle() const override;
            [[nodiscard]] WindowSize getWindowSize() const override;

            void setIcon(const std::string &path) const override;

            [[nodiscard]] bool shouldClose() const override { return m_shouldClose; }
            void pollEvents() override;
            bool pollEvent(WindowEvent &event) override;

            bool wasResized() const override { return m_frameBufferResized; }
            void resetResizedFlag() override { m_frameBufferResized = false; }

        private:
            static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
            static KeyCode mapWinKey(WPARAM key);
            std::queue<WindowEvent> m_eventQueue;

            std::wstring m_title;
            HWND m_hwnd = nullptr;
            HINSTANCE m_hInstance = nullptr;
            WindowSize m_frameBufferSize;
            bool m_frameBufferResized = false;
            bool m_shouldClose = false;

    }; // class Win32
} // namespace cae