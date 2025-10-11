#include <filesystem>

#include "CAE/Application.hpp"
#include "CAE/ArgsHandler.hpp"
#include "Utils/Logger.hpp"

int main(const int argc, const char *const *argv, const char *const *envp)
{
    std::unique_ptr<cae::Application> app = nullptr;
    cae::ArgsConfig argsConfig;

    utl::Logger::init();
    try
    {
        argsConfig = cae::ArgsHandler::ParseArgs(argc, argv);
        if (!argsConfig.run)
        {
            return EXIT_SUCCESS;
        }
        app = std::make_unique<cae::Application>(argsConfig, cae::ArgsHandler::ParseEnv(envp));
        app->start();
        app->stop();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
