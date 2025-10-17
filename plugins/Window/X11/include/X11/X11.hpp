///
/// @file X11.hpp
/// @brief This file contains the X11 class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/IWindow.hpp"

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

            [[nodiscard]] void *getNativeHandle() const override { return m_window; }
            [[nodiscard]] WindowSize getWindowSize() const override;

            [[nodiscard]] bool setIcon(const std::string &path) const override;

            [[nodiscard]] bool shouldClose() const override;
            void pollEvents() override;

            bool wasResized() const override { return m_frameBufferResized; }
            void resetResizedFlag() override { m_frameBufferResized = false; }

        private:
            void *m_window = nullptr;
            WindowSize m_frameBufferSize;
            mutable bool m_frameBufferResized = false;

    }; // class GLFW
} // namespace cae