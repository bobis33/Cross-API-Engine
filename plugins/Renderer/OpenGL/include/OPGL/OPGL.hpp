///
/// @file OPGL.hpp
/// @brief This file contains the OPGL class declaration
/// @namespace cae
///

#pragma once

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

            void initialize(void *nativeWindowHandle) override {}
    }; // class OPGL
} // namespace cae