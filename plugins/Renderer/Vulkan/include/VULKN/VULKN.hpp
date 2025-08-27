///
/// @file VULKN.hpp
/// @brief This file contains the VULKN class declaration
/// @namespace CAE
///

#pragma once

#include "CAE/Interfaces/IRenderer.hpp"


namespace CAE
{

    ///
    /// @class VULKN
    /// @brief Class for the Vulkan plugin
    /// @namespace CAE
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

        [[nodiscard]] const char* getName() const override { return "Vulkan"; }
        [[nodiscard]] PluginType getType() const override { return PluginType::RENDERER; }

    }; // class VULKN

} // namespace CAE