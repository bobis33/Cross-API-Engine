#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
    #define PLUGINS_EXTENSION ".dll"
    #define APP_NAME "cae.exe"
#else
    #define PLUGINS_EXTENSION ".so"
    #define APP_NAME "cae"
#endif

#include "CAE/PluginManager.hpp"
#include "CAE/Generated/Version.hpp"
#include "Utils/Logger.hpp"

static constexpr std::string_view HELP_MSG =
    "Usage: ./" APP_NAME " [options]\n\n"
    "Options:\n"
    "  -h, --help       Show this help message\n"
    "  -v, --version    Show version information\n";

static constexpr std::string_view APP_VERSION = PROJECT_NAME " v" PROJECT_VERSION
    " (" GIT_TAG ", commit " GIT_COMMIT_HASH ")\n"
    "Git tag: " GIT_TAG "\n" "Git commit: " GIT_COMMIT_HASH "\n"
    "Build type: " BUILD_TYPE "\n" "Build date: " __DATE__ "\n" "Build time: " __TIME__ "\n";

int main(const int argc, const char *const argv[], const char *const envp[]) {
    const std::filesystem::path pluginDir{PLUGINS_DIR};
    if (argc > 1) {
        const std::string arg1{argv[1]};
        if (arg1 == "-h" || arg1 == "--help") {
            std::cout << HELP_MSG;
            return EXIT_SUCCESS;
        }
        if (arg1 == "-v" || arg1 == "--version") {
            std::cout << APP_VERSION;
            return EXIT_SUCCESS;
        }
        std::cerr << "Unknown argument: " << arg1 << "\n";
        std::cerr << "Use -h or --help to see available options.\n";
        return EXIT_FAILURE;
    }
    try {
        utl::Logger::init();
        cae::PluginManager pluginManager;
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
