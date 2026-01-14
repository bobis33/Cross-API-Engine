///
/// @file IRenderer.hpp
/// @brief This file contains the Renderer interface
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/IModel.hpp"
#include "Interfaces/Renderer/IShader.hpp"
#include "Interfaces/IWindow.hpp"

#include <memory>

namespace cae
{

    ///
    /// @interface IRenderer
    /// @brief Interface for renderer
    /// @namespace cae
    ///
    class IRenderer : public utl::IPlugin
    {

        public:
            ~IRenderer() override = default;

            virtual std::shared_ptr<IShader> getShader() const = 0;
            virtual std::shared_ptr<IModel> getModel() const = 0;

            virtual void setShader(std::shared_ptr<IShader> shader) = 0;
            virtual void setModel(std::shared_ptr<IModel> model) = 0;

            virtual void initialize(const NativeWindowHandle &nativeWindowHandle, std::shared_ptr<IShader> shader) = 0;
            virtual void createPipeline(const ShaderPipelineDesc& pipeline) = 0;

            virtual void draw(const WindowSize &windowSize) = 0;

            virtual void setVSyncEnabled(bool enabled) = 0;
            virtual bool isVSyncEnabled() const = 0;

    }; // interface IRenderer

} // namespace cae
