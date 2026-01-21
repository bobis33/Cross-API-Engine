///
/// @file DirectX11.hpp
/// @brief This file contains the DirectX11 class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/ARenderer.hpp"

#include <glm/glm.hpp>

namespace cae
{

    ///
    /// @class DirectX11
    /// @brief Class for the DirectX11 plugin
    /// @namespace cae
    ///
    class DirectX11 final : public ARenderer
    {

        public:
            DirectX11() = default;
            ~DirectX11() override = default;

            DirectX11(const DirectX11 &) = delete;
            DirectX11 &operator=(const DirectX11 &) = delete;
            DirectX11(DirectX11 &&) = delete;
            DirectX11 &operator=(DirectX11 &&) = delete;

            [[nodiscard]] std::string getName() const override { return "DirectX11"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::RENDERER; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::WINDOWS; }

            void setVSyncEnabled(bool enabled) override {}
            void setClearColor(const Color &color) override {}

            [[nodiscard]] bool isVSyncEnabled() const override { return false; }

            void initialize(const NativeWindowHandle &nativeWindowHandle, const Color &clearColor) override {}
            void createPipeline(const ShaderID &id, const ShaderIRModule &vertex,
                                const ShaderIRModule &fragment) override
            {
            }
            void draw(const WindowSize &windowSize, const ShaderID &shaderId, glm::mat4 mvp) override {}
            void createMesh(const std::vector<float> &vertices) override {}

    }; // class DirectX11

} // namespace cae