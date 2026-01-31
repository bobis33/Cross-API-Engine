///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/IShaderCompiler.hpp"
#include "Interfaces/Input/Key/Keyboard.hpp"
#include "Interfaces/Input/Key/Mouse.hpp"

#include <glm/mat4x4.hpp>

namespace cae
{

    ///
    /// @struct Color
    /// @brief Struct for color
    /// @namespace cae
    ///
    struct Color
    {
            float r;
            float g;
            float b;
            float a;
    };

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
    /// @enum WindowEventType
    /// @brief Enum for window event types
    /// @namespace cae
    ///
    enum class WindowEventType : uint8_t
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
    /// @interface IRenderer
    /// @brief Interface for renderer
    /// @namespace cae
    ///
    class IRenderer : public utl::IPlugin
    {

        public:
            ~IRenderer() override = default;

            ///
            /// @param enabled Whether VSync is enabled
            /// @brief Enable or disable VSync
            ///
            virtual void setVSyncEnabled(bool enabled) = 0;

            ///
            /// @param color Clear color to set
            /// @brief Set the clear color
            ///
            virtual void setClearColor(const Color &color) = 0;

            ///
            /// @return Whether VSync is enabled
            /// @brief Check if VSync is enabled
            ///
            [[nodiscard]] virtual bool isVSyncEnabled() const = 0;

            ///
            /// @param clearColor Clear color (default: white)
            /// @brief Initialize the renderer with a native window handle and clear color
            ///
            virtual void initialize(const Color &clearColor = {.r = 1.F, .g = 1.F, .b = 1.F, .a = 1.F}) = 0;

            ///
            /// @param id Shader ID
            /// @param vertex Vertex shader IR module
            /// @param fragment Fragment shader IR module
            /// @brief Create a rendering pipeline with vertex and fragment shaders
            ///
            virtual void createPipeline(const ShaderID &id, const ShaderIRModule &vertex,
                                        const ShaderIRModule &fragment) = 0;

            ///
            /// @param windowSize Current window size
            /// @param shaderId Shader ID to use for drawing
            /// @param mvp Model-View-Projection matrix
            /// @brief Draw the scene using the specified shader and window size
            ///
            virtual void draw(const WindowSize &windowSize, const ShaderID &shaderId, glm::mat4 mvp) = 0;

            ///
            /// @param vertices Vertex data to create the mesh
            /// @brief Create a mesh with the given vertex data
            ///
            virtual void createMesh(const std::vector<float> &vertices) = 0;



            ///
            /// @param name Window name
            /// @param size Window size
            /// @return True if the window was created successfully
            /// @brief Create a window with the given name and size
            ///
            virtual bool createWindow(const std::string &name, WindowSize size) = 0;

            ///
            /// @brief Close the window
            ///
            virtual void closeWindow() = 0;

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
            virtual void setWindowIcon(const std::string &path) const = 0;

            ///
            /// @return True if the window should close
            /// @brief Check if the window should close
            ///
            virtual bool windowShouldClose() const = 0;

            ///
            /// @brief Poll window events
            ///
            virtual void windowPollEvents() = 0;

            ///
            /// @param outEvent Event to be filled
            /// @return True if an event was polled
            /// @brief Poll window events into outEvent
            ///
            virtual bool windowPollEvent(WindowEvent &outEvent) = 0;

            ///
            /// @return True if the window was resized
            /// @brief Check if the window was resized
            ///
            virtual bool windowWasResized() const = 0;

            ///
            /// @brief Reset the resized flag
            ///
            virtual void windowResetResizedFlag() = 0;

            // virtual bool isFullScreen() const = 0;
            // virtual void setFullScreen(bool fullScreen) const = 0;

    }; // interface IRenderer

} // namespace cae
