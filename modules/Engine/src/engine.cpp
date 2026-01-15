#include "Engine/Engine.hpp"

#include "Utils/Logger.hpp"

#include <numeric>
#include <sstream>

void printFps(std::array<float, 10> &fpsBuffer, int &fpsIndex, const float deltaTime)
{
    fpsBuffer[fpsIndex % 10] = 1.0F / deltaTime;
    fpsIndex++;

    float avgFps = std::accumulate(fpsBuffer.begin(), fpsBuffer.end(), 0.0f) / 10.0f;
    utl::Logger::log(std::format("FPS: {}", avgFps), utl::LogLevel::INFO);
}

cae::Engine::Engine(const EngineConfig &config, const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                    const std::function<std::shared_ptr<IInput>()> &inputFactory,
                    const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                    const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                    const std::function<std::shared_ptr<IShaderIR>()> &shaderIRFactory,
                    const std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> &shaderFrontendFactories,
                    const std::function<std::shared_ptr<IWindow>()> &windowFactory,
                    const std::vector<ShaderSourceDesc> &shaderSources, const std::vector<float> &vertices)
    : m_audioPlugin(audioFactory()), m_inputPlugin(inputFactory()), m_networkPlugin(networkFactory()),
      m_rendererPlugin(rendererFactory()), m_windowPlugin(windowFactory()),
      m_clock(std::make_unique<utl::Clock>()), m_shaderManager(std::make_unique<ShaderManager>()), m_camera(std::make_unique<Camera>())
{
    constexpr auto boolToStr = [](const bool b) { return b ? "true" : "false"; };
    std::ostringstream msg;
    msg << "Loading engine with configuration:\n"
        << "\tAudio master volume: " << config.audio_master_volume << "\n"
        << "\tAudio muted: " << boolToStr(config.audio_muted) << "\n"
        << "\tNetwork host: " << config.network_host << "\n"
        << "\tNetwork port: " << config.network_port << "\n"
        << "\tRenderer vsync: " << boolToStr(config.renderer_vsync) << "\n"
        << "\tRenderer frame rate limit: " << config.renderer_frame_rate_limit << "\n"
        << "\tRenderer clear color: (" << config.renderer_clear_color.r << ", " << config.renderer_clear_color.g << ", "
        << config.renderer_clear_color.b << ", " << config.renderer_clear_color.a << ")\n"
        << "\tWindow width: " << config.window_width << "\n"
        << "\tWindow height: " << config.window_height << "\n"
        << "\tWindow fullscreen: " << boolToStr(config.window_fullscreen) << "\n"
        << "\tWindow name: " << config.window_name;
    utl::Logger::log(msg.str(), utl::LogLevel::INFO);

    initWindow(config.window_name, {.width = config.window_width, .height = config.window_height});
    initRenderer(m_windowPlugin->getNativeHandle(), vertices, config.renderer_clear_color);
    initShaders(shaderIRFactory, shaderFrontendFactories, shaderSources);
}

void cae::Engine::run() const
{
    std::array<float, 10> fpsBuffer{};
    int fpsIndex = 0;
    while (!m_windowPlugin->shouldClose())
    {
        m_rendererPlugin->draw(m_windowPlugin->getWindowSize(), "basic");
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

void cae::Engine::initWindow(const std::string &windowName, const WindowSize &windowSize)
{
    m_windowPlugin->create(windowName, windowSize);
}

void cae::Engine::initRenderer(const NativeWindowHandle &nativeWindowHandle, const std::vector<float> &vertices,
                               const Color &clearColor) const
{
    m_rendererPlugin->initialize(nativeWindowHandle, clearColor);
    m_rendererPlugin->createMesh(vertices);
}

void cae::Engine::initShaders(
    const std::function<std::shared_ptr<IShaderIR>()> &shaderIRFactory,
    const std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> &shaderFrontendFactories,
    const std::vector<ShaderSourceDesc> &shaderSources) const
{
    for (const auto &factory : shaderFrontendFactories)
    {
        auto frontend = factory();
        m_shaderManager->registerFrontend(frontend);
    }
    m_shaderManager->registerIR(shaderIRFactory());
    auto shaders = m_shaderManager->build(shaderSources, ShaderSourceType::SPIRV);
    m_shaderManager->optimizeAll(ShaderSourceType::SPIRV, shaders | std::views::values);
    m_rendererPlugin->createPipeline("basic", shaders["basic_vertex"], shaders["basic_fragment"]);
}
