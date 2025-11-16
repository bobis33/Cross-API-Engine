///
/// @file ArgsHandler.hpp
/// @brief This file contains the ArgsHandler class declaration
/// @namespace cae
///

#pragma once

#include <string>

namespace cae
{

    struct ArgsConfig
    {
            bool run = false;
            std::string config_path;
    };
    struct EnvConfig
    {
            std::string user_name;
            std::string pwd;
    };

    ///
    /// @class ArgsHandler
    /// @brief Class to handle command line arguments
    /// @namespace cae
    ///
    class ArgsHandler
    {

        public:
            ArgsHandler() = default;
            ~ArgsHandler() = default;

            ArgsHandler(const ArgsHandler &) = delete;
            ArgsHandler &operator=(const ArgsHandler &) = delete;
            ArgsHandler(ArgsHandler &&) = delete;
            ArgsHandler &operator=(ArgsHandler &&) = delete;

            static ArgsConfig ParseArgs(int argc, const char *const *argv);
            static EnvConfig ParseEnv(const char *const *envp);

        private:
    }; // class ArgsHandler

} // namespace cae
