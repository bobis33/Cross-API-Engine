#include <filesystem>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include "CAE/Application.hpp"
#include "CAE/Generated/Version.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

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

static cae::EngineConfig parseJsonConf(const std::string &path)
{
    const fs::path filePath(path);
    if (!fs::exists(filePath))
    {
        std::cerr << "Config file not found: " << filePath << '\n';
        return {};
    }
    if (!fs::is_regular_file(filePath))
    {
        std::cerr << "Config path is not a regular file: " << filePath << '\n';
        return {};
    }

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open config file: " << filePath << '\n';
        return {};
    }

    json j;
    try
    {
        file >> j;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "Failed to parse JSON config (" << filePath << "): " + std::string(e.what()) << '\n';
        return {};
    }
    cae::EngineConfig config;
    utl::Logger::log("Loading config: " + filePath.string(), utl::LogLevel::INFO);
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
    utl::Logger::log("PROJECT INFO:", utl::LogLevel::INFO);
    std::cout << "\tName: " PROJECT_NAME "\n"
              << "\tVersion: " PROJECT_VERSION "\n"
              << "\tBuild type: " BUILD_TYPE "\n"
              << "\tGit tag: " GIT_TAG "\n"
              << "\tGit commit hash: " GIT_COMMIT_HASH "\n";

    try
    {
        m_appConfig.envConfig = envConfig;

        if (!argsConfig.config_path.empty())
        {
            m_appConfig.engineConfig = parseJsonConf(argsConfig.config_path);
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
