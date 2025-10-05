///
/// @file ArgsHandler.hpp
/// @brief This file contains the ArgsHandler class declaration
/// @namespace cae
///

#pragma once

namespace cae
{

    struct ArgsConfig
    {
        bool run{false};
    };
    struct EnvConfig
    {
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

            ArgsConfig ParseArgs(int argc, const char *const *argv);
            EnvConfig ParseEnv(const char *const *envp);

        private:

    }; // class ArgsHandler

} // namespace cae