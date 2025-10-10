#include <filesystem>
#include <string>
#include <vector>

#include "CAE/Application.hpp"
#include "Utils/Interfaces/IPlugin.hpp"
#include "Utils/Logger.hpp"
#include "Utils/PluginLoader.hpp"

cae::Application::Application() : m_engine(std::make_unique<Engine>())
{
    try
    {
        const std::filesystem::path pluginDir{PLUGINS_DIR};
        utl::PluginLoader pluginManager;
        std::vector<std::string> loadedPlugins;
        for (const auto &entry : std::filesystem::directory_iterator(pluginDir))
        {
            if (!entry.is_regular_file() || entry.path().extension() != PLUGINS_EXTENSION)
            {
                continue;
            }
            if (const std::string pluginPath = entry.path().string();
                pluginManager.loadPlugin<utl::IPlugin>(pluginPath) != nullptr)
            {
                loadedPlugins.push_back(entry.path().filename().string());
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
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}