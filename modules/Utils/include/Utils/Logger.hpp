#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>


namespace utl {

    enum class LogLevel : uint8_t {
        INFO,
        WARNING
    };

    class Logger {

        public:
            Logger(const Logger &) = delete;
            Logger &operator=(const Logger &) = delete;
            Logger(Logger &&) = delete;
            Logger &operator=(Logger &&) = delete;

            static void init() {
    #ifdef _WIN32
                const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
                DWORD dwMode = 0;
                if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
                    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
                }
    #endif
            }

            template <typename Func>
            static void logExecutionTime(const std::string &message, Func &&func) {
                const auto start = std::chrono::high_resolution_clock::now();
                func();
                const auto end = std::chrono::high_resolution_clock::now();
                const auto duration = std::chrono::duration<float, std::milli>(end - start).count();

                std::cout << getColorForDuration(duration)
                          << formatLogMessage(LogLevel::INFO, message + " took " + std::to_string(duration) + " ms")
                          << LOG_LEVEL_COLOR[RESET];
            }

            static void logWarning(const std::string &message) {
                std::cout << LOG_LEVEL_COLOR[WARNING]
                          << formatLogMessage(LogLevel::WARNING, message)
                          << LOG_LEVEL_COLOR[RESET];
            }

            static void logInfo(const std::string &message) {
                std::cout << LOG_LEVEL_COLOR[INFO]
                          << formatLogMessage(LogLevel::INFO, message)
                          << LOG_LEVEL_COLOR[RESET];
            }

        private:
            enum ColorIndex : uint8_t { ERROR, INFO, WARNING, RESET };

            static constexpr std::array<const char *, 4> LOG_LEVEL_COLOR = {
                "\033[31m", // ERROR/slow execution
                "\033[32m", // INFO/fast execution
                "\033[33m", // WARNING/medium execution
                "\033[0m\n" // RESET + newline
            };

            static constexpr std::array<const char *, 2> LOG_LEVEL_STRING = {"INFO", "WARNING"};

            Logger() = default;
            ~Logger() = default;

            [[nodiscard]] static const char *getColorForDuration(const float duration) {
                return duration < 20.0f ? LOG_LEVEL_COLOR[INFO]
                                        : (duration < 90.0F ? LOG_LEVEL_COLOR[WARNING] : LOG_LEVEL_COLOR[ERROR]);
            }

            [[nodiscard]] static std::string formatLogMessage(LogLevel level, const std::string &message) {
                const auto inTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                std::ostringstream ss;
                ss << "[" << std::put_time(std::localtime(&inTime), "%Y-%m-%d %X") << "] ";
                ss << "[" << LOG_LEVEL_STRING[static_cast<uint8_t>(level)] << "] " << message;
                return ss.str();
            }

    }; // class Logger

} // namespace utl
