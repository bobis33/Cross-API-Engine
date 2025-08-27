///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace CAE
///

#pragma once

#include "CAE/Interfaces/IPlugin.hpp"

namespace CAE {

    ///
    /// @interface IRenderer
    /// @brief Interface for renderer
    /// @namespace CAE
    ///
    class IRenderer : public IPlugin {

        public:
            ~IRenderer() override = default;

    }; // interface IRenderer

} // namespace CAE
