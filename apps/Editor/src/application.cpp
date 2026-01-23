#include "CAE/Application.hpp"
#include "CAE/Common.hpp"

#include "Utils/File.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Path.hpp"

static std::vector<std::shared_ptr<cae::utl::IPlugin>> loadPlugins(const std::unique_ptr<cae::utl::PluginLoader> &loader)
{
    std::vector<std::shared_ptr<cae::utl::IPlugin>> loadedPlugins;
    const std::filesystem::path pluginDir{CAE_PLUGINS_DIR};
    if (!cae::utl::Path::existsDir(pluginDir))
    {
        cae::utl::Logger::log("Plugins directory does not exist: " + pluginDir.string(), cae::utl::LogLevel::WARNING);
        return loadedPlugins;
    }

    for (const auto &entry : std::filesystem::directory_iterator(pluginDir))
    {
        if (!entry.is_regular_file() || entry.path().extension() != PLUGINS_EXTENSION)
        {
            continue;
        }
        const std::string pluginPath = entry.path().string();
        if (auto plugin = loader->loadPlugin<cae::utl::IPlugin>(pluginPath, "cae-"); plugin != nullptr)
        {
            cae::utl::Logger::log("Loaded plugin:\t " + plugin->getName() + " from " + pluginPath, cae::utl::LogLevel::INFO);
            loadedPlugins.push_back(plugin);
        }
        else
        {
            cae::utl::Logger::log("Failed to load plugin: " + pluginPath, cae::utl::LogLevel::WARNING);
        }
    }
    if (loadedPlugins.empty())
    {
        cae::utl::Logger::log("No plugins loaded from directory: " + pluginDir.string(), cae::utl::LogLevel::WARNING);
    }

    return loadedPlugins;
}

cae::Application::Application(const ArgsConfig &argsConfig, const EnvConfig &envConfig)
    : m_pluginLoader(std::make_unique<utl::PluginLoader>())
{
    utl::Logger::log("PROJECT INFO:\n" + std::string(MESSAGE::VERSION_MSG) + '\n', utl::LogLevel::INFO);

    try
    {
        m_appConfig.envConfig = envConfig;

        if (!argsConfig.config_path.empty())
        {
            m_appConfig.engineConfig = parseEngineConf(argsConfig.config_path);
        }
        setupEngine(PLUGINS::NAME::RENDERER::OPENGL, PLUGINS::NAME::WINDOW::GLFW, PLUGINS::NAME::SHADER::FRONTEND::GLSL,
                    PLUGINS::NAME::SHADER::IR::SPIRV);
    }
    catch (const std::exception &e)
    {
        utl::Logger::log(std::string("Application initialization failed: ") + e.what(), utl::LogLevel::WARNING);
    }
}

void cae::Application::setupEngine(const std::string &rendererName, const std::string &windowName,
                                   const std::string &shaderFrontendName, const std::string &shaderIRName)
{
    std::shared_ptr<IWindow> windowPlugin = nullptr;
    std::shared_ptr<IRenderer> rendererPlugin = nullptr;
    std::shared_ptr<IShaderIR> shaderIRPlugin = nullptr;
    std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> shaderFactories;

    for (auto &plugin : loadPlugins(m_pluginLoader))
    {
        if (const auto renderer = std::dynamic_pointer_cast<IRenderer>(plugin))
        {
            if (renderer->getName() == rendererName)
            {
                rendererPlugin = renderer;
            }
        }
        if (const auto window = std::dynamic_pointer_cast<IWindow>(plugin))
        {
            if (window->getName() == windowName)
            {
                windowPlugin = window;
            }
        }
        if (const auto shader = std::dynamic_pointer_cast<IShaderFrontend>(plugin))
        {
            if (shader->getName() == shaderFrontendName)
            {
                shaderFactories.emplace_back([shader]() { return shader; });
            }
        }
        if (const auto shaderIR = std::dynamic_pointer_cast<IShaderIR>(plugin))
        {
            if (shaderIR->getName() == shaderIRName)
            {
                shaderIRPlugin = shaderIR;
            }
        }
    }
    if (windowPlugin == nullptr)
    {
        utl::Logger::log("No window plugin found with name: " + windowName, utl::LogLevel::WARNING);
    }
    if (rendererPlugin == nullptr)
    {
        utl::Logger::log("No renderer plugin found with name: " + rendererName, utl::LogLevel::WARNING);
    }
    if (shaderFactories.empty())
    {
        utl::Logger::log("No shader plugin found with name: " + shaderFrontendName, utl::LogLevel::WARNING);
    }
    m_engine = std::make_unique<eng::Engine>(
        m_appConfig.engineConfig, []() { return nullptr; }, []() { return nullptr; },
        [rendererPlugin]() { return rendererPlugin; }, [shaderIRPlugin]() { return shaderIRPlugin; }, shaderFactories,
        [windowPlugin]() { return windowPlugin; });
}

static const std::vector<float> cubeVertices = {
    // positions          // colors
    -0.5f, -0.5f, -0.5f, 1, 0, 0, 0.5f,  -0.5f, -0.5f, 0, 1, 0, 0.5f,  0.5f,  -0.5f, 0, 0, 1,
    0.5f,  0.5f,  -0.5f, 0, 0, 1, -0.5f, 0.5f,  -0.5f, 1, 1, 0, -0.5f, -0.5f, -0.5f, 1, 0, 0,

    -0.5f, -0.5f, 0.5f,  1, 0, 1, 0.5f,  -0.5f, 0.5f,  0, 1, 1, 0.5f,  0.5f,  0.5f,  1, 1, 1,
    0.5f,  0.5f,  0.5f,  1, 1, 1, -0.5f, 0.5f,  0.5f,  0, 0, 0, -0.5f, -0.5f, 0.5f,  1, 0, 1,

    -0.5f, 0.5f,  0.5f,  1, 0, 0, -0.5f, 0.5f,  -0.5f, 0, 1, 0, -0.5f, -0.5f, -0.5f, 0, 0, 1,
    -0.5f, -0.5f, -0.5f, 0, 0, 1, -0.5f, -0.5f, 0.5f,  1, 1, 0, -0.5f, 0.5f,  0.5f,  1, 0, 0,

    0.5f,  0.5f,  0.5f,  1, 0, 1, 0.5f,  0.5f,  -0.5f, 0, 1, 1, 0.5f,  -0.5f, -0.5f, 1, 1, 1,
    0.5f,  -0.5f, -0.5f, 1, 1, 1, 0.5f,  -0.5f, 0.5f,  0, 0, 0, 0.5f,  0.5f,  0.5f,  1, 0, 1,

    -0.5f, -0.5f, -0.5f, 1, 0, 0, 0.5f,  -0.5f, -0.5f, 0, 1, 0, 0.5f,  -0.5f, 0.5f,  0, 0, 1,
    0.5f,  -0.5f, 0.5f,  0, 0, 1, -0.5f, -0.5f, 0.5f,  1, 1, 0, -0.5f, -0.5f, -0.5f, 1, 0, 0,

    -0.5f, 0.5f,  -0.5f, 1, 0, 1, 0.5f,  0.5f,  -0.5f, 0, 1, 1, 0.5f,  0.5f,  0.5f,  1, 1, 1,
    0.5f,  0.5f,  0.5f,  1, 1, 1, -0.5f, 0.5f,  0.5f,  0, 0, 0, -0.5f, 0.5f,  -0.5f, 1, 0, 1};

void cae::Application::start()
{
    static const std::vector<ShaderSourceDesc> shaderSources = {
        {.id = "basic_vertex",
         .type = ShaderSourceType::GLSL,
         .source = utl::fileToString(utl::Path::resolveRelativeToExe("assets/shaders/glsl/texture.vert")),
         .stage = ShaderStage::VERTEX},

        {.id = "basic_fragment",
         .type = ShaderSourceType::GLSL,
         .source = utl::fileToString(utl::Path::resolveRelativeToExe("assets/shaders/glsl/texture.frag")),
         .stage = ShaderStage::FRAGMENT},
    };
    m_engine->initializeRenderResources(shaderSources, cubeVertices);
    mainLoop();
}

void cae::Application::stop()
{
    m_engine->stop();

    m_pluginLoader = nullptr;
    m_engine = nullptr;
}

void cae::Application::mainLoop()
{
    std::array<float, 10> fpsBuffer{};
    int fpsIndex = 0;
    WindowEvent e{};

    while (!m_engine->getWindow()->shouldClose())
    {
        m_engine->render();
        glm::vec3 moveDir(0.0F);
        glm::vec2 lookDir(0.0F);
        m_engine->getWindow()->pollEvents();
        while (m_engine->getWindow()->pollEvent(e))
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

        if (m_keyState[KeyCode::Up])
        {
            lookDir.y += 1.0F;
        }
        if (m_keyState[KeyCode::Down])
        {
            lookDir.y -= 1.0F;
        }
        if (m_keyState[KeyCode::Left])
        {
            lookDir.x -= 1.0F;
        }
        if (m_keyState[KeyCode::Right])
        {
            lookDir.x += 1.0F;
        }

        if (glm::length(lookDir) > 0.0F)
        {
            lookDir *= m_engine->getCamera()->getLookSpeed() * m_engine->getClock()->getDeltaSeconds();
            m_engine->getCamera()->rotate(lookDir.x, lookDir.y, 1.0F);
        }

        glm::vec3 forward = glm::normalize(
            glm::vec3(m_engine->getCamera()->getDirection().x, 0.0F, m_engine->getCamera()->getDirection().z));
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0F, 1.0F, 0.0F)));

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

        if (glm::length(moveDir) > 0.0F)
        {
            moveDir = glm::normalize(moveDir);
            m_engine->getCamera()->move(moveDir, m_engine->getClock()->getDeltaSeconds());
        }

        if (m_keyState[KeyCode::LCtrl])
        {
            m_engine->getCamera()->move(glm::vec3(0.0F, -1.0F, 0.0F), m_engine->getClock()->getDeltaSeconds());
        }
        if (m_keyState[KeyCode::Space])
        {
            m_engine->getCamera()->move(glm::vec3(0.0F, 1.0F, 0.0F), m_engine->getClock()->getDeltaSeconds());
        }

        m_engine->update(fpsBuffer, fpsIndex);
    }
}
