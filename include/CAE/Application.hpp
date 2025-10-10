///
/// @file Application.hpp
/// @brief This file contains the Application class declaration
/// @namespace cae
///

#pragma once
#include "Engine/Engine.hpp"

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
            Application();
            ~Application() = default;

            Application(const Application &) = delete;
            Application &operator=(const Application &) = delete;
            Application(Application &&) = delete;
            Application &operator=(Application &&) = delete;

        private:
            std::unique_ptr<Engine> m_engine;
    }; // class Application

} // namespace cae