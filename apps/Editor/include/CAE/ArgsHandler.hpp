///
/// @file ArgsHandler.hpp
/// @brief This file contains the ArgsHandler class declaration
/// @namespace cae
///

#pragma once

#include <string>

namespace cae
{

    ///
    /// @struct ArgsConfig
    /// @brief Struct for command line arguments configuration
    /// @namespace cae
    ///
    struct ArgsConfig
    {
            bool run = false;
            std::string config_path;
    };

    ///
    /// @struct EnvConfig
    /// @brief Struct for environment variables configuration
    /// @namespace cae
    ///
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

            ///
            /// @param argc argument count
            /// @param argv argument vector
            /// @return Parsed ArgsConfig
            /// @brief Parse command line arguments
            ///
            static ArgsConfig ParseArgs(int argc, const char *const *argv);

            ///
            /// @param envp environment pointer
            /// @return Parsed EnvConfig
            /// @brief Parse environment variables
            ///
            static EnvConfig ParseEnv(const char *const *envp);

        private:
    }; // class ArgsHandler

} // namespace cae
