///
/// @file Engine.hpp
/// @brief This file contains the engine class declaration
/// @namespace cae
///

#pragma once

#include "Engine/Camera.hpp"
#include "Engine/Common.hpp"
#include "Engine/ShaderManager.hpp"

#include "Interfaces/IAudio.hpp"
#include "Interfaces/INetwork.hpp"
#include "Interfaces/Input/IInput.hpp"
#include "Interfaces/Renderer/IRenderer.hpp"
#include "Utils/Clock.hpp"

#include <functional>

namespace cae
{

    struct EngineConfig
    {
            float audio_master_volume = Audio::VOLUME;
            bool audio_muted = Audio::MUTED;

            std::string network_host = Network::HOST;
            uint16_t network_port = Network::PORT;

            bool renderer_vsync = Renderer::VSYNC;
            uint16_t renderer_frame_rate_limit = Renderer::FRAME_RATE_LIMIT;
            Color renderer_clear_color = {.r = Renderer::CLEAR_COLOR_R,
                                          .g = Renderer::CLEAR_COLOR_G,
                                          .b = Renderer::CLEAR_COLOR_B,
                                          .a = Renderer::CLEAR_COLOR_A};

            uint16_t window_width = Window::WIDTH;
            uint16_t window_height = Window::HEIGHT;
            bool window_fullscreen = Window::FULLSCREEN;
            std::string window_name = Window::NAME;
            std::string window_icon_path = Window::ICON_PATH;
    };

    ///
    /// @class Engine
    /// @brief Engine class
    /// @namespace cae
    ///
    class Engine
    {

        public:
            Engine(const EngineConfig &config, const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                   const std::function<std::shared_ptr<IInput>()> &inputFactory,
                   const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                   const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                   const std::function<std::shared_ptr<IShaderIR>()> &shaderIRFactory,
                   const std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> &shaderFrontendFactories,
                   const std::function<std::shared_ptr<IWindow>()> &windowFactory);
            ~Engine() = default;

            Engine(const Engine &) = delete;
            Engine &operator=(const Engine &) = delete;
            Engine(Engine &&) = delete;
            Engine &operator=(Engine &&) = delete;

            [[nodiscard]] const std::shared_ptr<IAudio> &getAudio() const { return m_audioPlugin; }
            [[nodiscard]] const std::shared_ptr<IInput> &getInput() const { return m_inputPlugin; }
            [[nodiscard]] const std::shared_ptr<INetwork> &getNetwork() const { return m_networkPlugin; }
            [[nodiscard]] const std::shared_ptr<IRenderer> &getRenderer() const { return m_rendererPlugin; }
            [[nodiscard]] const std::shared_ptr<IWindow> &getWindow() const { return m_windowPlugin; }

            [[nodiscard]] const std::unique_ptr<utl::Clock> &getClock() { return m_clock; }
            [[nodiscard]] const std::unique_ptr<Camera> &getCamera() const { return m_camera; }

            void initializeRenderResources(const std::vector<ShaderSourceDesc> &shaderSources, const std::vector<float> &vertices) const;

            void run() const;
            void stop();

        private:
            std::shared_ptr<IAudio> m_audioPlugin = nullptr;
            std::shared_ptr<IInput> m_inputPlugin = nullptr;
            std::shared_ptr<INetwork> m_networkPlugin = nullptr;
            std::shared_ptr<IRenderer> m_rendererPlugin = nullptr;
            std::shared_ptr<IWindow> m_windowPlugin = nullptr;

            std::unique_ptr<utl::Clock> m_clock = nullptr;
            std::unique_ptr<ShaderManager> m_shaderManager = nullptr;
            std::unique_ptr<Camera> m_camera = nullptr;

            void initWindow(const std::string &windowName, const WindowSize &windowSize, const std::string &iconPath);
            void initShaders(const std::vector<ShaderSourceDesc> &shaderSources) const;

    }; // class Engine

} // namespace cae
