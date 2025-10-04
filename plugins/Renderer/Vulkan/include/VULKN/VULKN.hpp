///
/// @file VULKN.hpp
/// @brief This file contains the VULKN class declaration
/// @namespace cae
///

#pragma once

#include "CAE/Interfaces/IRenderer.hpp"

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

        [[nodiscard]] const std::string getName() const override { return "Vulkan"; }
        [[nodiscard]] const utl::PluginType getType() const override { return utl::PluginType::RENDERER; }

    }; // class VULKN

} // namespace cae