///
/// @file Cli.hpp
/// @brief This file contains the cli class declaration
/// @namespace cae
///

#pragma once

#include <functional>
#include <string>

namespace cae
{

    ///
    /// @class Cli
    /// @brief Cli class
    /// @namespace cae
    ///
    class Cli
    {

        public:
            Cli() = default;
            ~Cli() = default;

            Cli(const Cli &) = delete;
            Cli &operator=(const Cli &) = delete;
            Cli(Cli &&) = delete;
            Cli &operator=(Cli &&) = delete;

            void run();
            void registerCommand(const std::string& name, std::function<void()> callback);

        private:

    }; // class Cli

} // namespace cae
