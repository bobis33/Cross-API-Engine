///
/// @file Application.hpp
/// @brief This file contains the Application class declaration
/// @namespace cae
///

#pragma once

#include <Utils/PluginLoader.hpp>

#include "CAE/ArgsHandler.hpp"
#include "CAE/Engine/Engine.hpp"

namespace cae
{

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
            Application(const ArgsConfig &argsConfig, const EnvConfig &envConfig);
            ~Application() = default;

            Application(const Application &) = delete;
            Application &operator=(const Application &) = delete;
            Application(Application &&) = delete;
            Application &operator=(Application &&) = delete;

            void start() const;
            void stop();

        private:
            std::unique_ptr<utl::PluginLoader> m_pluginLoader = nullptr;
            std::unique_ptr<Engine> m_engine = nullptr;

            EngineConfig m_appConfig;
            EnvConfig m_envConfig;
    }; // class Application

} // namespace cae