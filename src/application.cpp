#include <filesystem>

#include "CAE/Application.hpp"

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
        setupEngine("Vulkan", "GLFW");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void cae::Application::setupEngine(const std::string &rendererName, const std::string &windowName)
{
    std::shared_ptr<IWindow> windowPlugin = nullptr;
    std::shared_ptr<IRenderer> rendererPlugin = nullptr;

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
    }
    if (windowPlugin == nullptr)
    {
        utl::Logger::log("No window plugin found with name: " + rendererName, utl::LogLevel::WARNING);
    }
    if (rendererPlugin == nullptr)
    {
        utl::Logger::log("No renderer plugin found with name: " + windowName, utl::LogLevel::WARNING);
    }
    m_engine = std::make_unique<Engine>(
        m_appConfig.engineConfig, []() { return nullptr; }, []() { return nullptr; }, []() { return nullptr; },
        [rendererPlugin]() { return rendererPlugin; }, [windowPlugin]() { return windowPlugin; });
}

void cae::Application::start() const { m_engine->run(); }

void cae::Application::stop()
{
    m_engine->stop();

    m_pluginLoader = nullptr;
    m_engine = nullptr;
}
