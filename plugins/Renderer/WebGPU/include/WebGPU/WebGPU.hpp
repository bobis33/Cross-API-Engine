///
/// @file WebGPU.hpp
/// @brief This file contains the WebGPU class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/ARenderer.hpp"

#include <glm/glm.hpp>

namespace cae
{

    ///
    /// @class WebGPU
    /// @brief Class for the WebGPU plugin
    /// @namespace cae
    ///
    class WebGPU final : public ARenderer
    {

        public:
            WebGPU() = default;
            ~WebGPU() override = default;

            WebGPU(const WebGPU &) = delete;
            WebGPU &operator=(const WebGPU &) = delete;
            WebGPU(WebGPU &&) = delete;
            WebGPU &operator=(WebGPU &&) = delete;

            [[nodiscard]] std::string getName() const override { return "WebGPU"; }
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

    }; // class WebGPU

} // namespace cae