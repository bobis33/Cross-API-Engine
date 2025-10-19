///
/// @file OPGL.hpp
/// @brief This file contains the OPGL class declaration
/// @namespace cae
///

#pragma once

#include <string>

#include "glad/glad.h"

#if defined(__linux__)
#include <GL/glx.h>
#include <X11/Xlib.h>
#endif

#include "Interfaces/IRenderer.hpp"

namespace cae
{
    ///
    /// @class OPGL
    /// @brief Class for the OpenGL plugin
    /// @namespace cae
    ///
    class OPGL final : public IRenderer
    {
        public:
            OPGL() = default;
            ~OPGL() override = default;

            OPGL(const OPGL &) = delete;
            OPGL &operator=(const OPGL &) = delete;
            OPGL(OPGL &&) = delete;
            OPGL &operator=(OPGL &&) = delete;

            [[nodiscard]] std::string getName() const override { return "OpenGL"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::RENDERER; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

            void initialize(const NativeWindowHandle &nativeWindowHandle) override;
            void draw(const WindowSize &windowSize) override;

        private:
#if defined(__linux__)
            Display *m_display = nullptr;
            Window m_window = 0;
            GLXContext m_context = nullptr;
#endif

            GLuint gVAO = 0;
            GLuint gVBO = 0;
            GLuint gShaderProgram = 0;

            void createShaderProgram();
            void createTriangle();
    }; // class OPGL
} // namespace cae
