///
/// @file Engine.hpp
/// @brief This file contains the engine class declaration
/// @namespace cae
///

#pragma once

#include "Engine/Camera.hpp"
#include "Engine/ShaderManager.hpp"

#include "Interfaces/Audio/IAudio.hpp"
#include "Interfaces/Network/INetwork.hpp"
#include "Interfaces/Renderer/IRenderer.hpp"
#include "Utils/Clock.hpp"

#include <glm/glm.hpp>

namespace cae
{

    ///
    /// @struct EngineConfig
    /// @brief Struct for engine configuration
    /// @namespace cae
    ///
    struct EngineConfig
    {
            float audio_master_volume = AUDIO::VOLUME;
            bool audio_muted = AUDIO::MUTED;

            glm::vec3 camera_position = glm::vec3(0.0F, 0.0F, 0.0F);
            glm::vec3 camera_rotation = glm::vec3(0.0F, 0.0F, 0.0F);
            glm::vec3 camera_direction = glm::vec3(0.0F, 0.0F, -1.0F);
            float camera_move_speed = CAMERA::MOVE_SPEED;
            float camera_look_speed = CAMERA::LOOK_SPEED;
            float camera_fov = CAMERA::FOV;
            float camera_near_plane = CAMERA::NEAR_PLANE;
            float camera_far_plane = CAMERA::FAR_PLANE;

            bool log_fps = LOG::LOG_FPS;

            std::string network_host = NETWORK::HOST;
            uint16_t network_port = NETWORK::PORT;

            bool renderer_vsync = RENDERER::VSYNC;
            uint16_t renderer_frame_rate_limit = RENDERER::FRAME_RATE_LIMIT;
            Color renderer_clear_color = {.r = RENDERER::CLEAR_COLOR_R,
                                          .g = RENDERER::CLEAR_COLOR_G,
                                          .b = RENDERER::CLEAR_COLOR_B,
                                          .a = RENDERER::CLEAR_COLOR_A};

            uint16_t window_width = WINDOW::WIDTH;
            uint16_t window_height = WINDOW::HEIGHT;
            bool window_fullscreen = WINDOW::FULLSCREEN;
            std::string window_name = WINDOW::NAME;
            std::string window_icon_path = WINDOW::ICON_PATH;
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
            [[nodiscard]] const std::shared_ptr<INetwork> &getNetwork() const { return m_networkPlugin; }
            [[nodiscard]] const std::shared_ptr<IRenderer> &getRenderer() const { return m_rendererPlugin; }
            [[nodiscard]] const std::shared_ptr<IWindow> &getWindow() const { return m_windowPlugin; }

            [[nodiscard]] const std::unique_ptr<utl::Clock> &getClock() { return m_clock; }
            [[nodiscard]] const std::unique_ptr<ShaderManager> &getShaderManager() const { return m_shaderManager; }
            [[nodiscard]] const std::unique_ptr<Camera> &getCamera() const { return m_camera; }

            ///
            /// @param shaderSources Shader sources to initialize
            /// @param vertices Vertex data to initialize
            /// @brief Initialize render resources
            ///
            void initializeRenderResources(const std::vector<ShaderSourceDesc> &shaderSources,
                                           const std::vector<float> &vertices) const;

            ///
            /// @param fpsBuffer
            /// @param fpsIndex
            /// @brief
            ///
            void update(std::array<float, 10> &fpsBuffer, int &fpsIndex);

            ///
            /// @brief
            ///
            void render();

            ///
            /// @brief Stop the engine
            ///
            void stop();

        private:
            std::shared_ptr<IAudio> m_audioPlugin = nullptr;
            std::shared_ptr<INetwork> m_networkPlugin = nullptr;
            std::shared_ptr<IRenderer> m_rendererPlugin = nullptr;
            std::shared_ptr<IWindow> m_windowPlugin = nullptr;

            std::unique_ptr<utl::Clock> m_clock = nullptr;
            std::unique_ptr<ShaderManager> m_shaderManager = nullptr;
            std::unique_ptr<Camera> m_camera = nullptr;

            bool m_logFps = false;

            ///
            /// @param windowName window name
            /// @param windowSize window size
            /// @param iconPath path to window icon
            /// @brief Initialize the window
            ///
            void initWindow(const std::string &windowName, const WindowSize &windowSize,
                            const std::string &iconPath) const;

            ///
            /// @param shaderSources Shader sources to initialize
            /// @brief Initialize shaders
            ///
            void initShaders(const std::vector<ShaderSourceDesc> &shaderSources) const;

    }; // class Engine

} // namespace cae
