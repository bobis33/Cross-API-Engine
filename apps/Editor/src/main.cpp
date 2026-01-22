#include "CAE/Application.hpp"

#include "Utils/Logger.hpp"

int main(const int argc, const char *const *argv, const char *const *envp)
{
    std::unique_ptr<cae::Application> app = nullptr;

    utl::Logger::init();
    try
    {
        cae::ArgsConfig argsConfig = cae::ArgsHandler::ParseArgs(argc, argv);
        cae::EnvConfig envConfig = cae::ArgsHandler::ParseEnv(envp);
        if (!argsConfig.run)
        {
            return EXIT_SUCCESS;
        }
        app = std::make_unique<cae::Application>(argsConfig, envConfig);
        app->start();
        app->stop();
    }
    catch (const std::exception &e)
    {
        utl::Logger::log(std::string("Exception: ") + e.what() + '\n', utl::LogLevel::WARNING);
        return EXIT_FAILURE;
    }
    catch (...)
    {
        utl::Logger::log(std::string("Unknown exception"), utl::LogLevel::WARNING);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
