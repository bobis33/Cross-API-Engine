///
/// @file IWindow.hpp
/// @brief This file contains the Window interface
/// @namespace cae
///

#pragma once

#include "Utils/Interfaces/IPlugin.hpp"

namespace cae
{

    struct WindowSize
    {
            uint16_t width;
            uint16_t height;
    };

    struct NativeWindowHandle
    {
            void *window;
            void *display;
    };

    ///
    /// @interface IWindow
    /// @brief Interface for window
    /// @namespace cae
    ///
    class IWindow : public utl::IPlugin
    {

        public:
            ~IWindow() override = default;

            virtual bool create(const std::string &name, WindowSize size) = 0;
            virtual void close() = 0;

            virtual NativeWindowHandle getNativeHandle() const = 0;
            virtual WindowSize getWindowSize() const = 0;

            virtual bool setIcon(const std::string &path) const = 0;

            virtual bool shouldClose() const = 0;
            virtual void pollEvents() = 0;

            virtual bool wasResized() const = 0;
            virtual void resetResizedFlag() = 0;

        private:
    }; // interface IWindow

} // namespace cae
