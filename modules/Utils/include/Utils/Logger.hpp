///
/// @file Logger.hpp
/// @brief This file contains the Logger class
/// @namespace utl
///

#pragma once

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <functional>
#include <mutex>

namespace utl
{

    enum class LogLevel : uint8_t
    {
        INFO,
        WARNING
    };

    ///
    /// @class Logger
    /// @brief Class for logging
    /// @namespace utl
    ///
    class Logger
    {

        private:
            template <typename E> static constexpr auto to_underlying(E e) noexcept
            {
                return static_cast<std::underlying_type_t<E>>(e);
            }

        public:
            using LogSink = std::function<void(const std::string&, LogLevel)>;

            Logger(const Logger &) = delete;
            Logger &operator=(const Logger &) = delete;
            Logger(Logger &&) = delete;
            Logger &operator=(Logger &&) = delete;

            ///
            /// @brief Initialize the logger
            ///
            static void init();

            static void addSink(LogSink sink) {
                std::scoped_lock lock(m_sinksMutex);
                m_sinks.push_back(std::move(sink));
            }

            ///
            /// @tparam Func Function to be measured
            /// @param message Message to be logged
            /// @param func Function to be measured
            /// @brief Log the execution time of a function
            ///
            template <typename Func> static void logExecutionTime(const std::string &message, Func &&func)
            {
                const auto start = std::chrono::high_resolution_clock::now();
                func();
                const auto end = std::chrono::high_resolution_clock::now();
                const auto duration = std::chrono::duration<float, std::milli>(end - start).count();

                std::cout << getColorForDuration(duration)
                          << formatLogMessage(LogLevel::INFO, message + " took " + std::to_string(duration) + " ms")
                          << LOG_LEVEL_COLOR[to_underlying(ColorIndex::COLOR_RESET)];
            }

            ///
            /// @param message Message to be logged
            /// @param logLevel Log level of the message
            /// @brief Log a message with a specific log level
            ///
            static void log(const std::string &message, const LogLevel &logLevel)
            {
                const std::string formatted = formatLogMessage(logLevel, message);

                std::cout << (logLevel == LogLevel::INFO ? LOG_LEVEL_COLOR[to_underlying(ColorIndex::COLOR_INFO)]
                                                         : LOG_LEVEL_COLOR[to_underlying(ColorIndex::COLOR_WARNING)])
                          << formatted
                          << LOG_LEVEL_COLOR[to_underlying(ColorIndex::COLOR_RESET)];

                std::scoped_lock lock(m_sinksMutex);
                for (auto &sink : m_sinks) {
                    sink(formatted, logLevel);
}
            }

        private:
            enum class ColorIndex : uint8_t
            {
                COLOR_ERROR = 0,
                COLOR_INFO = 1,
                COLOR_WARNING = 2,
                COLOR_RESET = 3
            };

            static constexpr std::array<const char *, 4> LOG_LEVEL_COLOR = {
                "\033[31m", // ERROR/slow execution
                "\033[32m", // INFO/fast execution
                "\033[33m", // WARNING/medium execution
                "\033[0m\n" // RESET + newline
            };

            static constexpr std::array<const char *, 2> LOG_LEVEL_STRING = {"INFO", "WARNING"};

            Logger() = default;
            ~Logger() = default;

            [[nodiscard]] static const char *getColorForDuration(const float duration)
            {
                return duration < 20.0F ? LOG_LEVEL_COLOR[to_underlying(ColorIndex::COLOR_INFO)]
                                        : (duration < 90.0F ? LOG_LEVEL_COLOR[to_underlying(ColorIndex::COLOR_WARNING)]
                                                            : LOG_LEVEL_COLOR[to_underlying(ColorIndex::COLOR_ERROR)]);
            }

            [[nodiscard]] static std::string formatLogMessage(LogLevel level, const std::string &message)
            {
                using namespace std::chrono;

                const auto now = system_clock::now();
                const auto inTimeT = system_clock::to_time_t(now);
                const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

                std::ostringstream ss;
                ss << "[" << std::put_time(std::localtime(&inTimeT), "%Y-%m-%d %H:%M:%S");
                ss << ":" << std::setfill('0') << std::setw(3) << ms.count() << "] ";
                ss << "[" << LOG_LEVEL_STRING[static_cast<uint8_t>(level)] << "] " << message;

                return ss.str();
            }

            static inline std::vector<LogSink> m_sinks;
            static inline std::mutex m_sinksMutex;

    }; // class Logger

} // namespace utl
