#include "Utils/Logger.hpp"

#include "CAE/Engine/Engine.hpp"

cae::Engine::Engine(const EngineConfig &config, const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                    const std::function<std::shared_ptr<IInput>()> &inputFactory,
                    const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                    const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                    const std::function<std::shared_ptr<IShader>()> &shaderFactory,
                    const std::function<std::shared_ptr<IWindow>()> &windowFactory)
    : m_audioPlugin(audioFactory()), m_inputPlugin(inputFactory()), m_networkPlugin(networkFactory()),
      m_rendererPlugin(rendererFactory()), m_shaderPlugin(shaderFactory()), m_windowPlugin(windowFactory()), m_clock(std::make_unique<utl::Clock>())
{
    utl::Logger::log("Loading engine with configuration:", utl::LogLevel::INFO);
    utl::Logger::log("\tAudio master volume: " + std::to_string(config.audio_master_volume), utl::LogLevel::INFO);
    utl::Logger::log("\tAudio muted: " + std::string(config.audio_muted ? "true" : "false"), utl::LogLevel::INFO);
    utl::Logger::log("\tNetwork host: " + config.network_host, utl::LogLevel::INFO);
    utl::Logger::log("\tNetwork port: " + std::to_string(config.network_port), utl::LogLevel::INFO);
    utl::Logger::log("\tRenderer vsync: " + std::string(config.renderer_vsync ? "true" : "false"), utl::LogLevel::INFO);
    utl::Logger::log("\tRenderer frame rate limit: " + std::to_string(config.renderer_frame_rate_limit),
                     utl::LogLevel::INFO);
    utl::Logger::log("\tWindow width: " + std::to_string(config.window_width), utl::LogLevel::INFO);
    utl::Logger::log("\tWindow height: " + std::to_string(config.window_height), utl::LogLevel::INFO);
    utl::Logger::log("\tWindow fullscreen: " + std::string(config.window_fullscreen ? "true" : "false"),
                     utl::LogLevel::INFO);
    utl::Logger::log("\tWindow name: " + config.window_name, utl::LogLevel::INFO);
    m_windowPlugin->create(config.window_name, {.width = config.window_width, .height = config.window_height});
    m_rendererPlugin->initialize(m_windowPlugin->getNativeHandle(), m_shaderPlugin);
}

void cae::Engine::run() const
{

    while (!m_windowPlugin->shouldClose())
    {
        utl::Logger::log(std::format("FPS: {}", 1.0F / m_clock->getDeltaSeconds()), utl::LogLevel::INFO);
        m_clock->restart();
        m_rendererPlugin->draw(m_windowPlugin->getWindowSize());
        m_windowPlugin->pollEvents();
    }
}

void cae::Engine::stop()
{
    utl::Logger::log("Stopping engine...", utl::LogLevel::INFO);
    m_windowPlugin->close();

    m_audioPlugin = nullptr;
    m_inputPlugin = nullptr;
    m_networkPlugin = nullptr;
    m_rendererPlugin = nullptr;
    m_windowPlugin = nullptr;
}
