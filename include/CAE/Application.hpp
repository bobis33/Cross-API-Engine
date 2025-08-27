///
/// @file Application.hpp
/// @brief This file contains the Application class declaration
/// @namespace CAE
///

#pragma once

namespace CAE
{

    ///
    /// @class Application
    /// @brief Main class
    /// @namespace CAE
    ///
    class Application
    {

        public:
            Application() = default;
            ~Application() = default;

            Application(const Application &) = delete;
            Application &operator=(const Application &) = delete;
            Application(Application &&) = delete;
            Application &operator=(Application &&) = delete;

    }; // class Application

} // namespace CAE