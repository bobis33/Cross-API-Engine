///
/// @file IWindow.hpp
/// @brief This file contains the Window interface
/// @namespace cae
///

#pragma once

#include "Input/Key/Keyboard.hpp"
#include "Input/Key/Mouse.hpp"
#include "Utils/Interfaces/IPlugin.hpp"

namespace cae
{

    ///
    /// @struct WindowSize
    /// @brief Struct for window size
    /// @namespace cae
    ///
    struct WindowSize
    {
            uint16_t width;
            uint16_t height;
    };

    ///
    /// @struct NativeWindowHandle
    /// @brief Struct for native window handle
    /// @namespace cae
    ///
    struct NativeWindowHandle
    {
            void *window;
            void *display;
    };

    ///
    /// @enum WindowEventType
    /// @brief Enum for window event types
    /// @namespace cae
    ///
    enum class WindowEventType
    {
        KeyDown,
        KeyUp,
        MouseMove,
        MouseButtonDown,
        MouseButtonUp,
        MouseScroll,
        Resize,
        Focus,
        Close
    };

    ///
    /// @struct WindowEvent
    /// @brief Struct for window events
    /// @namespace cae
    ///
    struct WindowEvent
    {
            WindowEventType type;

            union
            {
                    struct
                    {
                            KeyCode key;
                    } key;
                    struct
                    {
                            int x, y;
                    } mouseMove;
                    struct
                    {
                            MouseButton button;
                    } mouseButton;
                    struct
                    {
                            float x, y;
                    } scroll;
                    struct
                    {
                            uint16_t w, h;
                    } resize;
            };
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

            ///
            /// @param name Window name
            /// @param size Window size
            /// @return True if the window was created successfully
            /// @brief Create a window with the given name and size
            ///
            virtual bool create(const std::string &name, WindowSize size) = 0;

            ///
            /// @brief Close the window
            ///
            virtual void close() = 0;

            ///
            /// @return Native window handle
            /// @brief Get the native window handle
            ///
            virtual NativeWindowHandle getNativeHandle() const = 0;

            ///
            /// @return Current window size
            /// @brief Get the current window size
            ///
            virtual WindowSize getWindowSize() const = 0;

            ///
            /// @param path Path to the icon image
            /// @return True if the icon was set successfully
            /// @brief Set the window icon from the given image path
            ///
            virtual bool setIcon(const std::string &path) const = 0;

            ///
            /// @return True if the window should close
            /// @brief Check if the window should close
            ///
            virtual bool shouldClose() const = 0;

            ///
            /// @brief Poll window events
            ///
            virtual void pollEvents() = 0;

            ///
            /// @param outEvent Event to be filled
            /// @return True if an event was polled
            /// @brief Poll window events into outEvent
            ///
            virtual bool pollEvent(WindowEvent &outEvent) = 0;

            ///
            /// @return True if the window was resized
            /// @brief Check if the window was resized
            ///
            virtual bool wasResized() const = 0;

            ///
            /// @brief Reset the resized flag
            ///
            virtual void resetResizedFlag() = 0;

            // virtual bool isFullScreen() const = 0;
            // virtual void setFullScreen(bool fullScreen) const = 0;

        private:
            // std::unique_ptr<IInput> m_inputManager;

    }; // interface IWindow

} // namespace cae
