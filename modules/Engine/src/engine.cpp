#include "Engine/Engine.hpp"

#include "Utils/Logger.hpp"

#include <numeric>
#include <ranges>

#include "Utils/Path.hpp"

void printFps(std::array<float, 10> &fpsBuffer, int &fpsIndex, const float deltaTime)
{
    fpsBuffer[fpsIndex % 10] = 1.0F / deltaTime;
    fpsIndex++;

    float avgFps = std::accumulate(fpsBuffer.begin(), fpsBuffer.end(), 0.0f) / 10.0f;
    utl::Logger::log(std::format("FPS: {}", avgFps), utl::LogLevel::INFO);
}

cae::Engine::Engine(const EngineConfig &config, const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                    const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                    const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                    const std::function<std::shared_ptr<IShaderIR>()> &shaderIRFactory,
                    const std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> &shaderFrontendFactories,
                    const std::function<std::shared_ptr<IWindow>()> &windowFactory)
    : m_audioPlugin(audioFactory()), m_networkPlugin(networkFactory()), m_rendererPlugin(rendererFactory()),
      m_windowPlugin(windowFactory()), m_clock(std::make_unique<utl::Clock>()),
      m_shaderManager(std::make_unique<ShaderManager>(shaderFrontendFactories, shaderIRFactory)),
      m_camera(std::make_unique<Camera>(config.camera_position, config.camera_rotation, config.camera_direction,
                                        config.camera_move_speed, config.camera_look_speed, config.camera_fov,
                                        config.camera_near_plane, config.camera_far_plane)),
      m_logFps(config.log_fps)
{
    constexpr auto boolToStr = [](const bool b) { return b ? "true" : "false"; };
    std::ostringstream msg;
    msg << "Starting engine with configuration:\n"
        << "\tAudio master volume: " << config.audio_master_volume << "\n"
        << "\tAudio muted: " << boolToStr(config.audio_muted) << "\n"
        << "\tLog FPS: " << boolToStr(config.log_fps) << "\n"
        << "\tNetwork host: " << config.network_host << "\n"
        << "\tNetwork port: " << config.network_port << "\n"
        << "\tRenderer vsync: " << boolToStr(config.renderer_vsync) << "\n"
        << "\tRenderer frame rate limit: " << config.renderer_frame_rate_limit << "\n"
        << "\tRenderer clear color: (" << config.renderer_clear_color.r << ", " << config.renderer_clear_color.g << ", "
        << config.renderer_clear_color.b << ", " << config.renderer_clear_color.a << ")\n"
        << "\tWindow width: " << config.window_width << "\n"
        << "\tWindow height: " << config.window_height << "\n"
        << "\tWindow fullscreen: " << boolToStr(config.window_fullscreen) << "\n"
        << "\tWindow name: " << config.window_name << "\n\tWindow icon path: " << config.window_icon_path << '\n';
    utl::Logger::log(msg.str(), utl::LogLevel::INFO);

    initWindow(config.window_name, {.width = config.window_width, .height = config.window_height},
               config.window_icon_path);
    m_rendererPlugin->initialize(m_windowPlugin->getNativeHandle(), config.renderer_clear_color);
}

void cae::Engine::initializeRenderResources(const std::vector<ShaderSourceDesc> &shaderSources,
                                            const std::vector<float> &vertices) const
{
    initShaders(shaderSources);
    m_rendererPlugin->createMesh(vertices);
}

void cae::Engine::run()
{
    std::array<float, 10> fpsBuffer{};
    int fpsIndex = 0;
    WindowEvent e{};
    constexpr auto model = glm::mat4(1.0F);
    while (!m_windowPlugin->shouldClose())
    {
        const glm::mat4 mvp = m_camera->getViewProjection(static_cast<float>(m_windowPlugin->getWindowSize().width) /
                                                          m_windowPlugin->getWindowSize().height) *
                              model;
        m_rendererPlugin->draw(m_windowPlugin->getWindowSize(), "basic", mvp);
        m_windowPlugin->pollEvents();
        while (m_windowPlugin->pollEvent(e))
        {
            if (e.type == WindowEventType::KeyDown)
            {
                m_keyState[e.key.key] = true;
            }
            else if (e.type == WindowEventType::KeyUp)
            {
                m_keyState[e.key.key] = false;
            }
        }
        glm::vec3 moveDir(0.0F);
        glm::vec2 lookDir(0.0F);

        if (m_keyState[KeyCode::Up])
        {
            lookDir.y += 1.0f;
        }
        if (m_keyState[KeyCode::Down])
        {
            lookDir.y -= 1.0f;
        }
        if (m_keyState[KeyCode::Left])
        {
            lookDir.x -= 1.0f;
        }
        if (m_keyState[KeyCode::Right])
        {
            lookDir.x += 1.0f;
        }

        if (glm::length(lookDir) > 0.0f)
        {
            lookDir *= m_camera->getLookSpeed() * m_clock->getDeltaSeconds();
            m_camera->rotate(lookDir.x, lookDir.y, 1.0f);
        }

        glm::vec3 forward = glm::normalize(glm::vec3(m_camera->getDirection().x, 0.0f, m_camera->getDirection().z));
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

        if (m_keyState[KeyCode::W])
        {
            moveDir += forward;
        }
        if (m_keyState[KeyCode::S])
        {
            moveDir -= forward;
        }
        if (m_keyState[KeyCode::A])
        {
            moveDir -= right;
        }
        if (m_keyState[KeyCode::D])
        {
            moveDir += right;
        }

        if (glm::length(moveDir) > 0.0f)
        {
            moveDir = glm::normalize(moveDir);
            m_camera->move(moveDir, m_clock->getDeltaSeconds());
        }

        if (m_keyState[KeyCode::LCtrl])
        {
            m_camera->move(glm::vec3(0.0f, -1.0f, 0.0f), m_clock->getDeltaSeconds());
        }
        if (m_keyState[KeyCode::Space])
        {
            m_camera->move(glm::vec3(0.0f, 1.0f, 0.0f), m_clock->getDeltaSeconds());
        }

        if (m_logFps)
        {
            printFps(fpsBuffer, fpsIndex, m_clock->getDeltaSeconds());
        }
        m_clock->restart();
    }
}

void cae::Engine::stop()
{
    utl::Logger::log("Stopping engine...", utl::LogLevel::INFO);
    m_windowPlugin->close();

    m_audioPlugin = nullptr;
    m_networkPlugin = nullptr;
    m_rendererPlugin = nullptr;
    m_windowPlugin = nullptr;
}

void cae::Engine::initWindow(const std::string &windowName, const WindowSize &windowSize,
                             const std::string &iconPath) const
{
    m_windowPlugin->create(windowName, windowSize);
    if (!iconPath.empty())
    {
        m_windowPlugin->setIcon(iconPath);
    }
}

void cae::Engine::initShaders(const std::vector<ShaderSourceDesc> &shaderSources) const
{
    auto shaders = m_shaderManager->build(shaderSources, ShaderSourceType::SPIRV);
    m_shaderManager->optimizeAll(ShaderSourceType::SPIRV, shaders | std::views::values);
    m_rendererPlugin->createPipeline("basic", shaders["basic_vertex"], shaders["basic_fragment"]);
}
