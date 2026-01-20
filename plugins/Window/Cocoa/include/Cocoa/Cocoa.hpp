///
/// @file Cocoa.hpp
/// @brief This file contains the hpp class declaration
/// @namespace cae
///

#pragma once

#ifdef __APPLE__

#include "Interfaces/Window/AWindow.hpp"

#include <queue>

namespace cae
{

    ///
    /// @class Cocoa
    /// @brief Class for the Cocoa plugin
    /// @namespace cae
    ///
    class Cocoa final : public AWindow
    {
        public:
            Cocoa() = default;
            ~Cocoa() override;

            Cocoa(const Cocoa&) = delete;
            Cocoa& operator=(const Cocoa&) = delete;

            [[nodiscard]] std::string getName() const override { return "Cocoa"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::WINDOW; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::MACOSX; }

            bool create(const std::string& name, WindowSize size) override;
            void close() override;

            [[nodiscard]] NativeWindowHandle getNativeHandle() const override;
            [[nodiscard]] WindowSize getWindowSize() const override;

             void setIcon(const std::string &path) const override {}

            [[nodiscard]] bool shouldClose() const override;
            void pollEvents() override;
            bool pollEvent(WindowEvent& event) override;

            [[nodiscard]] bool wasResized() const override { return m_resized; }
            void resetResizedFlag() override { m_resized = false; }

        private:
            void* m_window = nullptr;   // NSWindow*
            void* m_view   = nullptr;   // NSView*
            void* m_app    = nullptr;   // NSApplication*

            bool m_shouldClose = false;
            bool m_resized = false;

            std::queue<WindowEvent> m_eventQueue;
            WindowSize m_size{};

    }; // class Cocoa

} // namespace cae

#endif
