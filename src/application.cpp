#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "CAE/Application.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

std::vector<std::shared_ptr<utl::IPlugin>> loadPlugins(const std::unique_ptr<utl::PluginLoader> &loader)
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
        auto plugin = loader->loadPlugin<utl::IPlugin>(pluginPath);
        if (plugin != nullptr)
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

cae::EngineConfig parseJsonConf(const std::string &path)
{
    if (path.empty())
    {
        return {};
    }
    const fs::path filePath(path);
    std::cout << filePath.string() << "\n";
    if (!fs::exists(filePath))
    {
        std::cerr << "Config file not found: " + std::string(filePath) << std::endl;
        return {};
    }

    if (!fs::is_regular_file(filePath))
    {
        std::cerr << "Config path is not a regular file: " + std::string(filePath) << std::endl;
        return {};
    }

    if (const auto ext = filePath.extension().string(); ext != ".json" && ext != ".JSON")
    {
        std::cerr << "Config file must have a .json extension: " + std::string(filePath) << std::endl;
        return {};
    }
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " + std::string(filePath) << std::endl;
        return {};
    }

    json j;
    try
    {
        file >> j;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "Failed to parse JSON config (" + std::string(filePath) + "): " + std::string(e.what()) << std::endl;
        return {};
    }
    cae::EngineConfig config;
    utl::Logger::log("Loading config: " + std::string(filePath), utl::LogLevel::INFO);
    if (j.contains("audio"))
    {
        const auto &audio = j["audio"];
        if (audio.contains("masterVolume") && audio["masterVolume"].is_number())
        {
            config.audio_master_volume = audio["masterVolume"];
        }
        if (audio.contains("muted") && audio["muted"].is_boolean())
        {
            config.audio_muted = audio["muted"];
        }
    }
    if (j.contains("network"))
    {
        const auto &network = j["network"];
        if (network.contains("host") && network["host"].is_string())
        {
            config.network_host = network["host"];
        }
        if (network.contains("port") && network["port"].is_number_unsigned())
        {
            config.network_port = network["port"];
        }
    }
    if (j.contains("renderer"))
    {
        const auto &renderer = j["renderer"];
        if (renderer.contains("vsync") && renderer["vsync"].is_boolean())
        {
            config.renderer_vsync = renderer["vsync"];
        }
        if (renderer.contains("frameRateLimit") && renderer["frameRateLimit"].is_number_unsigned())
        {
            config.renderer_frame_rate_limit = renderer["frameRateLimit"];
        }
    }
    if (j.contains("window"))
    {
        const auto &window = j["window"];
        if (window.contains("width") && window["width"].is_number_unsigned())
        {
            config.window_width = window["width"];
        }
        if (window.contains("height") && window["height"].is_number_unsigned())
        {
            config.window_height = window["height"];
        }
        if (window.contains("fullscreen") && window["fullscreen"].is_boolean())
        {
            config.window_fullscreen = window["fullscreen"];
        }
        if (window.contains("name") && window["name"].is_string())
        {
            config.window_name = window["name"];
        }
    }

    return config;
}

cae::Application::Application(const ArgsConfig &argsConfig, const EnvConfig &envConfig)
    : m_pluginLoader(std::make_unique<utl::PluginLoader>())
{
    std::shared_ptr<IWindow> windowPlugin = nullptr;
    std::shared_ptr<IRenderer> rendererPlugin = nullptr;
    AppConfig appConfig;

    try
    {
        appConfig.envConfig = envConfig;

        if (argsConfig.config_path != "")
        {
            appConfig.engineConfig = parseJsonConf(argsConfig.config_path);
        }
        for (auto &plugin : loadPlugins(m_pluginLoader))
        {
            if (const auto renderer = std::dynamic_pointer_cast<IRenderer>(plugin))
            {
                if (renderer->getName() == "Vulkan")
                {
                    rendererPlugin = renderer;
                }
            }
            if (const auto window = std::dynamic_pointer_cast<IWindow>(plugin))
            {
                if (window->getName() == "GLFW")
                {
                    windowPlugin = window;
                }
            }
        }
        m_engine = std::make_unique<Engine>(appConfig.engineConfig,
                                            []() { return nullptr; }, []() { return nullptr; },
                                            []() { return nullptr; }, [rendererPlugin]() { return rendererPlugin; },
                                            [windowPlugin]() { return windowPlugin; });
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void cae::Application::start() const { m_engine->run(); }

void cae::Application::stop()
{
    m_engine->stop();

    m_pluginLoader = nullptr;
    m_engine = nullptr;
}
