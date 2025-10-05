///
/// @file OPGL.hpp
/// @brief This file contains the OPGL class declaration
/// @namespace cae
///

#pragma once

#include "CAE/Interfaces/IRenderer.hpp"


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

        [[nodiscard]] const std::string getName() const override { return "OpenGL"; }
        [[nodiscard]] const utl::PluginType getType() const override { return utl::PluginType::RENDERER; }

    }; // class OPGL

} // namespace cae