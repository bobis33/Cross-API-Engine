///
/// @file ARenderer.hpp
/// @brief This file contains the Renderer abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/IRenderer.hpp"

#include <memory>

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

            std::shared_ptr<IModel> getModel() const override { return m_model; }

            void setModel(const std::shared_ptr<IModel> model) override { m_model = model; }

        protected:
            std::shared_ptr<IModel> m_model;

    }; // interface ARenderer

} // namespace cae
