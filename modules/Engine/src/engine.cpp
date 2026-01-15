#include "Engine/Engine.hpp"

#include "Utils/Logger.hpp"
#include "Utils/Utils.hpp"

#include <numeric>

cae::Engine::Engine(const EngineConfig &config, const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                    const std::function<std::shared_ptr<IInput>()> &inputFactory,
                    const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                    const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                    const std::function<std::shared_ptr<IShaderIR>()> &shaderIRFactory,
                    const std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> &shaderFactories,
                    const std::function<std::shared_ptr<IWindow>()> &windowFactory)
    : m_audioPlugin(audioFactory()), m_inputPlugin(inputFactory()), m_networkPlugin(networkFactory()),
      m_rendererPlugin(rendererFactory()), m_shaderManager(std::make_unique<ShaderManager>()),
      m_irManager(std::make_unique<ShaderIRManager>()), m_windowPlugin(windowFactory()),
      m_clock(std::make_unique<utl::Clock>())
{
    for (const auto &factory : shaderFactories)
    {
        auto frontend = factory();
        m_shaderManager->registerFrontend(frontend);
    }
    m_irManager->registerPlugin(shaderIRFactory());
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
    const std::vector<ShaderSourceDesc> shaderSources = {
        {.id = "basic_vertex",
         .type = ShaderSourceType::GLSL,
         .source = utl::fileToString("assets/shaders/uniform_color.vert"),
         .stage = ShaderStage::VERTEX},

        {.id = "basic_fragment",
         .type = ShaderSourceType::GLSL,
         .source = utl::fileToString("assets/shaders/uniform_color.frag"),
         .stage = ShaderStage::FRAGMENT},
    };

    for (const auto &src : shaderSources)
    {
        m_shaderManager->addSource(src);
    }

    m_shaderManager->compileAll();
    for (const auto &[id, irModule] : m_shaderManager->getAllIR())
    {
        const auto processed = m_irManager->process(irModule, "Vulkan");
        m_finalModules[id] = processed;
    }
    m_rendererPlugin->initialize(m_windowPlugin->getNativeHandle());

    m_rendererPlugin->createPipeline({.id = "basic", .vertex = "basic_vertex", .fragment = "basic_fragment"},
                                     m_finalModules["basic_vertex"], m_finalModules["basic_fragment"]);
}

void printFps(std::array<float, 10> &fpsBuffer, int &fpsIndex, const float deltaTime)
{
    fpsBuffer[fpsIndex % 10] = 1.0f / deltaTime;
    fpsIndex++;

    float avgFps = std::accumulate(fpsBuffer.begin(), fpsBuffer.end(), 0.0f) / 10.0f;
    utl::Logger::log(std::format("FPS: {}", avgFps), utl::LogLevel::INFO);
}

void cae::Engine::run() const
{
    std::array<float, 10> fpsBuffer{};
    int fpsIndex = 0;
    while (!m_windowPlugin->shouldClose())
    {
        m_rendererPlugin->draw(m_windowPlugin->getWindowSize());
        m_windowPlugin->pollEvents();
        printFps(fpsBuffer, fpsIndex, m_clock->getDeltaSeconds());
        m_clock->restart();
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
