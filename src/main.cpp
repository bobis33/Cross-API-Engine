#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #define PLUGINS_EXTENSION ".dll"
#else
    #define PLUGINS_EXTENSION ".so"
#endif
#include "CAE/PluginManager.hpp"
#include "Utils/Logger.hpp"

int main(int argc, const char *const argv[], const char *const envp[]) {
    const std::filesystem::path pluginDir{PLUGINS_DIR};
    try {
        utl::Logger::init();
        CAE::PluginManager pluginManager;
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
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
