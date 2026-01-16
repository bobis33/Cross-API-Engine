///
/// @file ARenderer.hpp
/// @brief This file contains the Renderer abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/IRenderer.hpp"

namespace cae
{

    ///
    /// @interface ARenderer
    /// @brief Abstract class for renderer
    /// @namespace cae
    ///
    class ARenderer : public IRenderer
    {

        public:
            ~ARenderer() override = default;

        protected:

        private:
    }; // interface ARenderer

} // namespace cae
