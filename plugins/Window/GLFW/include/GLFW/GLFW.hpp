///
/// @file GLFW.hpp
/// @brief This file contains the GLFW class declaration
/// @namespace cae
///

#pragma once

#include <GLFW/glfw3.h>

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include "Interfaces/IWindow.hpp"

namespace cae
{

    ///
    /// @class GLFW
    /// @brief Class for the GLFW plugin
    /// @namespace cae
    ///
    class GLFW final : public IWindow
    {

        public:
            GLFW() = default;
            ~GLFW() override = default;

            GLFW(const GLFW &) = delete;
            GLFW &operator=(const GLFW &) = delete;
            GLFW(GLFW &&) = delete;
            GLFW &operator=(GLFW &&) = delete;

            [[nodiscard]] std::string getName() const override { return "GLFW"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::WINDOW; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

            bool create(const std::string &name, WindowSize size) override;
            void close() override;

            [[nodiscard]] NativeWindowHandle getNativeHandle() const override;
            [[nodiscard]] WindowSize getWindowSize() const override;

            [[nodiscard]] bool setIcon(const std::string &path) const override;

            [[nodiscard]] bool shouldClose() const override { return glfwWindowShouldClose(m_window) != 0; }
            void pollEvents() override { glfwPollEvents(); }

            bool wasResized() const override { return m_frameBufferResized; }
            void resetResizedFlag() override { m_frameBufferResized = false; }

        private:
            static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);

            GLFWwindow *m_window = nullptr;
            WindowSize m_frameBufferSize;
            bool m_frameBufferResized = false;

    }; // class GLFW
} // namespace cae