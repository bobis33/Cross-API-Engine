#include <filesystem>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #define PLUGINS_EXTENSION ".dll"
#else
    #define PLUGINS_EXTENSION ".so"
#endif

#include "CAE/Application.hpp"
#include "CAE/PluginManager.hpp"
#include "Utils/Logger.hpp"

cae::Application::Application()
{
    try {
        const std::filesystem::path pluginDir{PLUGINS_DIR};
        PluginManager pluginManager;
        std::vector<std::string> loadedPlugins;
        for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
            if (!entry.is_regular_file() || entry.path().extension() != PLUGINS_EXTENSION) { continue; }
            if (const std::string pluginPath = entry.path().string(); pluginManager.loadPlugin(pluginPath) != nullptr) {
                loadedPlugins.push_back(entry.path().filename().string());
            } else {
                utl::Logger::logWarning("Failed to load plugin: " + pluginPath);
            }
        }
        if (loadedPlugins.empty()) {
            utl::Logger::logWarning("No plugins loaded from directory: " + pluginDir.string());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}