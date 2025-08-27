///
/// @file OPGL.hpp
/// @brief This file contains the OPGL class declaration
/// @namespace CAE
///

#pragma once

#include "CAE/Interfaces/IRenderer.hpp"

namespace CAE
{

    ///
    /// @class OPGL
    /// @brief Class for the OpenGL plugin
    /// @namespace CAE
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

        [[nodiscard]] const char* getName() const override { return "OpenGL"; }
        [[nodiscard]] PluginType getType() const override { return PluginType::RENDERER; }

    }; // class OPGL

} // namespace CAE