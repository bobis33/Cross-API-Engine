///
/// @file Vulkan.hpp
/// @brief This file contains the Vulkan class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/ARenderer.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace cae
{

    ///
    /// @class Vulkan
    /// @brief Class for the Vulkan plugin
    /// @namespace cae
    ///
    class Vulkan final : public ARenderer
    {

        public:
            Vulkan() = default;
            ~Vulkan() override = default;

            Vulkan(const Vulkan &) = delete;
            Vulkan &operator=(const Vulkan &) = delete;
            Vulkan(Vulkan &&) = delete;
            Vulkan &operator=(Vulkan &&) = delete;

            [[nodiscard]] std::string getName() const override { return "Vulkan"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::RENDERER; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

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

        private:

    }; // class Vulkan

} // namespace cae
