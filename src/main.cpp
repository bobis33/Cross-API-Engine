#include <filesystem>

#include "CAE/Application.hpp"
#include "CAE/ArgsHandler.hpp"
#include "Utils/Logger.hpp"

int main(const int argc, const char *const *argv, const char *const *envp) {
    utl::Logger::init();
    cae::ArgsHandler argsHandler{};
    try {
        auto [run] = argsHandler.ParseArgs(argc, argv);
        argsHandler.ParseEnv(envp);
        if (!run) { return EXIT_SUCCESS; }
        cae::Application app;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
