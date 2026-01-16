#include "CAE/Application.hpp"
#include "CAE/Common.hpp"
#include "Utils/Utils.hpp"

#include <filesystem>

static std::vector<std::shared_ptr<utl::IPlugin>> loadPlugins(const std::unique_ptr<utl::PluginLoader> &loader)
{
    const std::filesystem::path pluginDir{PLUGINS_DIR};
    std::vector<std::shared_ptr<utl::IPlugin>> loadedPlugins;

    for (const auto &entry : std::filesystem::directory_iterator(pluginDir))
    {
        if (!entry.is_regular_file() || entry.path().extension() != PLUGINS_EXTENSION)
        {
            continue;
        }
        const std::string pluginPath = entry.path().string();
        if (auto plugin = loader->loadPlugin<utl::IPlugin>(pluginPath); plugin != nullptr)
        {
            loadedPlugins.push_back(plugin);
        }
        else
        {
            utl::Logger::log("Failed to load plugin: " + pluginPath, utl::LogLevel::WARNING);
        }
    }
    if (loadedPlugins.empty())
    {
        utl::Logger::log("No plugins loaded from directory: " + pluginDir.string(), utl::LogLevel::WARNING);
    }

    return loadedPlugins;
}

cae::Application::Application(const ArgsConfig &argsConfig, const EnvConfig &envConfig)
    : m_pluginLoader(std::make_unique<utl::PluginLoader>())
{
    utl::Logger::log("PROJECT INFO:\n" + std::string(Message::VERSION_MSG), utl::LogLevel::INFO);

    try
    {
        m_appConfig.envConfig = envConfig;

        if (!argsConfig.config_path.empty())
        {
            m_appConfig.engineConfig = parseEngineConf(argsConfig.config_path);
        }
        setupEngine("OpenGL", "GLFW", "GLSL", "SPIRV");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void cae::Application::setupEngine(const std::string &rendererName, const std::string &windowName,
                                   const std::string &shaderFrontendName, const std::string &shaderIRName)
{
    std::shared_ptr<IWindow> windowPlugin = nullptr;
    std::shared_ptr<IRenderer> rendererPlugin = nullptr;
    std::shared_ptr<IShaderIR> shaderIRPlugin = nullptr;
    std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> shaderFactories;

    static const std::vector<ShaderSourceDesc> shaderSources = {
        {.id = "basic_vertex",
         .type = ShaderSourceType::GLSL,
         .source = utl::fileToString("assets/shaders/glsl/texture.vert"),
         .stage = ShaderStage::VERTEX},

        {.id = "basic_fragment",
         .type = ShaderSourceType::GLSL,
         .source = utl::fileToString("assets/shaders/glsl/texture.frag"),
         .stage = ShaderStage::FRAGMENT},
    };

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
    m_engine = std::make_unique<Engine>(
        m_appConfig.engineConfig, []() { return nullptr; }, []() { return nullptr; }, []() { return nullptr; },
        [rendererPlugin]() { return rendererPlugin; }, [shaderIRPlugin]() { return shaderIRPlugin; }, shaderFactories,
        [windowPlugin]() { return windowPlugin; }, shaderSources,
        std::vector{-0.5F, -0.5F, 1.F, 0.F, 0.F, 0.5F, -0.5F, 0.F, 1.F, 0.F, 0.F, 0.5F, 0.F, 0.F, 1.F});
}

void cae::Application::start() const { m_engine->run(); }

void cae::Application::stop()
{
    m_engine->stop();

    m_pluginLoader = nullptr;
    m_engine = nullptr;
}
