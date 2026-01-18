///
/// @file X11.hpp
/// @brief This file contains the X11 class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/IWindow.hpp"

#include <X11/Xlib.h>

#include <queue>

namespace cae
{

    ///
    /// @class X11
    /// @brief Class for the X11 plugin
    /// @namespace cae
    ///
    class X11 final : public IWindow
    {

        public:
            X11() = default;
            ~X11() override = default;

            X11(const X11 &) = delete;
            X11 &operator=(const X11 &) = delete;
            X11(X11 &&) = delete;
            X11 &operator=(X11 &&) = delete;

            [[nodiscard]] std::string getName() const override { return "X11"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::WINDOW; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::LINUX; }

            bool create(const std::string &name, WindowSize size) override;
            void close() override;

            [[nodiscard]] NativeWindowHandle getNativeHandle() const override
            {
                return {.window = reinterpret_cast<void *>(m_window), .display = reinterpret_cast<void *>(m_display)};
            }
            [[nodiscard]] WindowSize getWindowSize() const override;

            [[nodiscard]] bool setIcon(const std::string &path) const override;

            [[nodiscard]] bool shouldClose() const override;
            void pollEvents() override;
            bool pollEvent(WindowEvent& outEvent) override;

            bool wasResized() const override { return m_frameBufferResized; }
            void resetResizedFlag() override { m_frameBufferResized = false; }

        private:

            std::queue<WindowEvent> m_eventQueue;

            WindowSize m_frameBufferSize;
            mutable bool m_frameBufferResized = false;

            Display *m_display = nullptr;
            Window m_window = 0;
            Atom m_wmDeleteMessage = 0;
            bool m_shouldClose = false;

    }; // class X11

} // namespace cae
