///
/// @file VULKN.hpp
/// @brief This file contains the VULKN class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/IRenderer.hpp"

namespace cae
{

    ///
    /// @class VULKN
    /// @brief Class for the Vulkan plugin
    /// @namespace cae
    ///
    class VULKN final : public IRenderer
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

            void initialize(const NativeWindowHandle &nativeWindowHandle) override {}
            void draw(const WindowSize &windowSize) override {};

            void setVSyncEnabled(bool enabled) override {}
            [[nodiscard]] bool isVSyncEnabled() const override { return false; }
    }; // class VULKN
} // namespace cae