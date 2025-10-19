#include <Utils/Logger.hpp>

#include "CAE/Engine/Engine.hpp"

cae::Engine::Engine(const EngineConfig &config, const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                    const std::function<std::shared_ptr<IInput>()> &inputFactory,
                    const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                    const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                    const std::function<std::shared_ptr<IWindow>()> &windowFactory)
    : m_audioPlugin(audioFactory()), m_inputPlugin(inputFactory()), m_networkPlugin(networkFactory()),
      m_rendererPlugin(rendererFactory()), m_windowPlugin(windowFactory()), m_clock(std::make_unique<utl::Clock>()),
      m_config(config)
{
    std::ostringstream oss;
    oss << "Loading engine with configuration:\n"
        << "\tAudio master volume: " << config.audio_master_volume << "\n"
        << "\tAudio muted: " << (config.audio_muted ? "true" : "false") << "\n"
        << "\tNetwork host: " << config.network_host << "\n"
        << "\tNetwork port: " << config.network_port << "\n"
        << "\tRenderer vsync: " << (config.renderer_vsync ? "true" : "false") << "\n"
        << "\tRenderer frame rate limit: " << config.renderer_frame_rate_limit << "\n"
        << "\tWindow width: " << config.window_width << "\n"
        << "\tWindow height: " << config.window_height << "\n"
        << "\tWindow fullscreen: " << (config.window_fullscreen ? "true" : "false") << "\n"
        << "\tWindow name: " << config.window_name;
    utl::Logger::log(oss.str(), utl::LogLevel::INFO);
}

void cae::Engine::run()
{
    m_windowPlugin->create(m_config.window_name, {.width = m_config.window_width, .height = m_config.window_height});
    m_rendererPlugin->initialize(m_windowPlugin->getNativeHandle());
    while (!m_windowPlugin->shouldClose() && !m_restart)
    {
        utl::Logger::log(std::format("FPS: {}", 1.0F / m_clock->getDeltaSeconds()), utl::LogLevel::INFO);
        m_clock->restart();
        m_rendererPlugin->draw(m_windowPlugin->getWindowSize());
        m_windowPlugin->pollEvents();
    }
    m_windowPlugin->close();
    if (m_restart)
    {
        m_restart = false;
        run();
    }
}

void cae::Engine::stop()
{
    utl::Logger::log("Stopping engine...", utl::LogLevel::INFO);

    m_audioPlugin = nullptr;
    m_inputPlugin = nullptr;
    m_networkPlugin = nullptr;
    m_rendererPlugin = nullptr;
    m_windowPlugin = nullptr;
}

void cae::Engine::restart() { m_restart = true; }
