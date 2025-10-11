#include <functional>
#include <iostream>
#include <unordered_map>

#ifdef _WIN32
#define APP_EXTENSION ".exe"
#else
#define APP_EXTENSION ""
#endif

#include "Utils/Env.hpp"

#include "CAE/ArgsHandler.hpp"
#include "CAE/Generated/Version.hpp"

static constexpr std::string_view HELP_MSG = "Usage: " PROJECT_NAME APP_EXTENSION " [options]\n\n"
                                             "Options:\n"
                                             "  -h, --help              Show this help message\n"
                                             "  -v, --version           Show version information\n"
                                             "  -c, --config <path>     Specify JSON configuration file\n";

static constexpr std::string_view VERSION_MSG = PROJECT_NAME
    " v" PROJECT_VERSION " " BUILD_TYPE " (" GIT_TAG ", commit " GIT_COMMIT_HASH ") " __DATE__ " " __TIME__ "\n";

static const std::unordered_map<std::string, std::function<void()>> ARGS_MAP = {
    {"-h", []() { std::cout << HELP_MSG; }},
    {"--help", []() { std::cout << HELP_MSG; }},
    {"-v", []() { std::cout << VERSION_MSG; }},
    {"--version", []() { std::cout << VERSION_MSG; }},
    {"-c", []() {}},
    {"--config", []() {}}};

cae::ArgsConfig cae::ArgsHandler::ParseArgs(const int argc, const char *const *argv)
{
    ArgsConfig config;
    config.run = true;

    if (argc <= 1)
        return config;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            std::cout << HELP_MSG;
            config.run = false;
            return config;
        }
        if (arg == "-v" || arg == "--version")
        {
            std::cout << VERSION_MSG;
            config.run = false;
            return config;
        }
        if (arg == "-c" || arg == "--config")
        {
            if (i + 1 >= argc)
                throw std::runtime_error("Missing value for argument " + arg);

            config.config_path = argv[++i];
        }
        else
        {
            throw std::runtime_error("Unknown argument: " + arg + ". Use -h or --help to see available options.");
        }
    }

    return config;
}

cae::EnvConfig cae::ArgsHandler::ParseEnv(const char *const *envp)
{
    auto envMap = utl::getEnvMap(envp);
    return {};
}