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
    /// @class Application
    /// @brief Main class
    /// @namespace cae
    ///
    class Application
    {

        public:
            Application(ArgsConfig argsConfig, EnvConfig envConfig);
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
    }; // class Application

} // namespace cae