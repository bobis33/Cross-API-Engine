#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#endif
#include "CAE/PluginManager.hpp"
#include "CAE/Interfaces/IRenderer.hpp"

int main(int argc, const char *const argv[], const char *const envp[]) {
    const std::filesystem::path pluginDir{PLUGINS_DIR};
    try {
        CAE::PluginManager pluginManager;
        std::vector<std::string> loadedPlugins;
        for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
            if (!entry.is_regular_file() || entry.path().extension() != ".so") { continue; }

            const std::string pluginPath = entry.path().string();
            if (const auto *const plugin = pluginManager.loadPlugin(pluginPath)) {
                loadedPlugins.push_back(pluginPath);
            }
        }


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
