#include <functional>
#include <iostream>
#include <unordered_map>

#ifdef _WIN32
    #define APP_EXTENSION ".exe"
#else
    #define APP_EXTENSION ""
#endif

#include "CAE/ArgsHandler.hpp"
#include "CAE/Generated/Version.hpp"

static constexpr std::string_view HELP_MSG =
    "Usage: " PROJECT_NAME APP_EXTENSION " [options]\n\n"
    "Options:\n"
    "  -h, --help       Show this help message\n"
    "  -v, --version    Show version information\n";

static constexpr std::string_view VERSION_MSG =
    PROJECT_NAME " v" PROJECT_VERSION " " BUILD_TYPE " (" GIT_TAG ", commit " GIT_COMMIT_HASH ") " __DATE__ " " __TIME__ "\n";

static const std::unordered_map<std::string, std::function<void()>> ARGS_MAP = {
    {"-h" , []() { std::cout << HELP_MSG; }},
    {"--help", []() { std::cout << HELP_MSG; }},
    {"-v", []() { std::cout << VERSION_MSG; }},
    {"--version", []() { std::cout << VERSION_MSG; }}
};

cae::ArgsConfig cae::ArgsHandler::ParseArgs(const int argc, const char *const *argv)
{
    if (argc <= 1)
    {
        return { .run = true };
    }
    const std::string arg1{argv[1]};
    if (const auto it = ARGS_MAP.find(arg1); it != ARGS_MAP.end())
    {
        it->second();
        return { .run = false };
    }
    throw std::runtime_error("Unknown argument: " + arg1 + ". Use -h or --help to see available options.");
}

cae::EnvConfig cae::ArgsHandler::ParseEnv(const char *const *envp)
{
    (void)envp; // Currently unused
    return {};
}