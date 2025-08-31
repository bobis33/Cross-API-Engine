#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "CAE/Application.hpp"
#include "CAE/ArgsHandler.hpp"
#include "Utils/Logger.hpp"

int main(const int argc, const char *const argv[], const char *const envp[]) {
    utl::Logger::init();
    cae::ArgsHandler argsHandler{};
    try {
        cae::ArgsConfig conf = argsHandler.ParseArgs(argc, argv);
        argsHandler.ParseEnv(envp);
        if (!conf.run) { return EXIT_SUCCESS; }
        cae::Application app;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
