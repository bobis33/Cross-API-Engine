///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace cae
///

#pragma once

#include "CAE/Interfaces/IPlugin.hpp"

namespace cae
{

    ///
    /// @interface IRenderer
    /// @brief Interface for renderer
    /// @namespace cae
    ///
    class IRenderer : public IPlugin {

        public:
            ~IRenderer() override = default;

    }; // interface IRenderer

} // namespace cae
