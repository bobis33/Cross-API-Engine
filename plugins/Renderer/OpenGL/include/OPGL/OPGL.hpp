///
/// @file OPGL.hpp
/// @brief This file contains the OPGL class declaration
/// @namespace cae
///

#pragma once

#include "OPGL/glad/glad.h"

#include "Context/IContext.hpp"
#include "Interfaces/Renderer/ARenderer.hpp"

namespace cae
{

    ///
    /// @class OPGL
    /// @brief Class for the OpenGL plugin
    /// @namespace cae
    ///
    class OPGL final : public ARenderer
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

            void initialize(const NativeWindowHandle &window, std::shared_ptr<IShader> shader) override;
            void draw(const WindowSize &windowSize) override;

            void setVSyncEnabled(bool enabled) override;
            [[nodiscard]] bool isVSyncEnabled() const override;

        private:
            std::unique_ptr<IContext> m_context;
            std::shared_ptr<IShader> m_shader;

            GLuint gVAO = 0;
            GLuint gVBO = 0;
            GLuint gShaderProgram = 0;

            void createShaderProgram();
            void createTriangle();

    }; // class OPGL

} // namespace cae
