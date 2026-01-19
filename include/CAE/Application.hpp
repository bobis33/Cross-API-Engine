///
/// @file Application.hpp
/// @brief This file contains the Application class declaration
/// @namespace cae
///

#pragma once

#include "CAE/ArgsHandler.hpp"

#include "Engine/Engine.hpp"
#include "Utils/PluginLoader.hpp"

namespace cae
{

    ///
    /// @struct AppConfig
    /// @brief Struct for application configuration
    /// @namespace cae
    ///
    struct AppConfig
    {
            EngineConfig engineConfig;
            EnvConfig envConfig;
    };

    ///
    /// @class Application
    /// @brief Main class
    /// @namespace cae
    ///
    class Application
    {

        public:
            ///
            /// @param argsConfig Command line arguments configuration
            /// @param envConfig Environment variables configuration
            /// @brief Construct the Application with given configurations
            ///
            Application(const ArgsConfig &argsConfig, const EnvConfig &envConfig);
            ~Application() = default;

            Application(const Application &) = delete;
            Application &operator=(const Application &) = delete;
            Application(Application &&) = delete;
            Application &operator=(Application &&) = delete;

            ///
            /// @brief Start the application
            ///
            void start();

            ///
            /// @brief Stop the application
            ///
            void stop();

        private:
            ///
            /// @param rendererName renderer plugin name
            /// @param windowName window plugin name
            /// @param shaderFrontendName shader frontend plugin name
            /// @param shaderIRName shader IR plugin name
            /// @brief Setup the engine with the given plugins
            ///
            void setupEngine(const std::string &rendererName, const std::string &windowName,
                             const std::string &shaderFrontendName, const std::string &shaderIRName);

            ///
            /// @param path Path to the engine configuration file
            /// @return Parsed EngineConfig
            /// @brief Parse the engine configuration file
            ///
            static EngineConfig parseEngineConf(const std::string &path);

            void mainLoop();

            std::unique_ptr<utl::PluginLoader> m_pluginLoader = nullptr;
            std::unique_ptr<Engine> m_engine = nullptr;

            AppConfig m_appConfig;
            std::unordered_map<KeyCode, bool> m_keyState;


    }; // class Application

} // namespace cae
