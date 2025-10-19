///
/// @file CommandLine.hpp
/// @brief This file contains the CommandLine class declaration
/// @namespace cae
///

#pragma once

#include <atomic>
#include <thread>

#include "CAE/Engine/Engine.hpp"

namespace cae
{

    ///
    /// @class CommandLine
    /// @brief CommandLine class
    /// @namespace cae
    ///
    class CommandLine
    {

        public:
            explicit CommandLine(const std::unique_ptr<Engine> &engine) : m_engine(engine) {}
            ~CommandLine() = default;

            CommandLine(const CommandLine &) = delete;
            CommandLine &operator=(const CommandLine &) = delete;
            CommandLine(CommandLine &&) = delete;
            CommandLine &operator=(CommandLine &&) = delete;

            void run();
            void stop();

        private:
            const std::unique_ptr<Engine> &m_engine;
            std::atomic<bool> m_running = true;
            std::thread m_thread;

    }; // class CommandLine
} // namespace cae