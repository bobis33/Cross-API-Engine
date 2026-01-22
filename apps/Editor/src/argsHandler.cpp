#include "CAE/ArgsHandler.hpp"
#include "CAE/Common.hpp"

#include "Utils/Env.hpp"

#include <iostream>

cae::ArgsConfig cae::ArgsHandler::ParseArgs(const int argc, const char *const *argv)
{
    ArgsConfig config;
    config.run = true;

    if (argc <= 1)
    {
        return config;
    }

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            std::cout << MESSAGE::HELP_MSG;
            config.run = false;
            return config;
        }
        if (arg == "-v" || arg == "--version")
        {
            std::cout << MESSAGE::VERSION_MSG;
            config.run = false;
            return config;
        }
        if (arg == "-c" || arg == "--config")
        {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value for argument " + arg);
            }

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
    EnvConfig config;
    const auto envMap = utl::getEnvMap(envp);

    if (envMap.contains("USER"))
    {
        config.user_name = envMap.at("USER");
    }
    if (envMap.contains("PWD"))
    {
        config.pwd = envMap.at("PWD");
    }

    return config;
}