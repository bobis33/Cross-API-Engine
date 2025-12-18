///
/// @file VULKN.hpp
/// @brief This file contains the VULKN class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/ARenderer.hpp"

namespace cae
{

    ///
    /// @class VULKN
    /// @brief Class for the Vulkan plugin
    /// @namespace cae
    ///
    class VULKN final : public ARenderer
    {

        public:
            VULKN() = default;
            ~VULKN() override = default;

            VULKN(const VULKN &) = delete;
            VULKN &operator=(const VULKN &) = delete;
            VULKN(VULKN &&) = delete;
            VULKN &operator=(VULKN &&) = delete;

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
            void draw(const WindowSize &windowSize, const ShaderID &shaderId) override {}
            void createMesh(const std::vector<float> &vertices) override {}

    }; // class VULKN

} // namespace cae