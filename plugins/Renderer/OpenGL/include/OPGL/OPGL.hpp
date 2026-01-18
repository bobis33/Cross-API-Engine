///
/// @file OPGL.hpp
/// @brief This file contains the OPGL class declaration
/// @namespace cae
///

#pragma once

#include "OPGL/Context/IContext.hpp"

#include "Interfaces/Renderer/ARenderer.hpp"

#include <memory>
#include <unordered_map>

namespace cae
{

    struct Mesh
    {
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
            GLsizei vertexCount = 0;
    };

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

            void setVSyncEnabled(const bool enabled) override { m_context->setVSyncEnabled(enabled); }
            void setClearColor(const Color &color) override
            {
                auto &gl = m_context->gl;
                gl.ClearColor(color.r, color.g, color.b, color.a);
            }

            [[nodiscard]] bool isVSyncEnabled() const override { return m_context->isVSyncEnabled(); }

            void initialize(const NativeWindowHandle &nativeWindowHandle, const Color &clearColor) override;
            void createPipeline(const ShaderID &id, const ShaderIRModule &vertex,
                                const ShaderIRModule &fragment) override;
            void draw(const WindowSize &windowSize, const ShaderID &shaderId) override;
            void createMesh(const std::vector<float> &vertices) override;

        private:
            std::unique_ptr<IContext> m_context;
            std::unordered_map<ShaderID, GLuint> m_programs;
            Mesh m_mesh;

            GladGLContext m_device{};

            static GLuint createGLShader(GLenum type, const ShaderIRModule &data, GladGLContext gl);

    }; // class OPGL

} // namespace cae
