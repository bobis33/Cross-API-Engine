///
/// @file Console.hpp
/// @brief This file contains the console class declaration
/// @namespace cae
///

#pragma once
#include "Interfaces/Window/IWindow.hpp"
#include "Utils/Logger.hpp"

#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

namespace cae
{

    ///
    /// @class Console
    /// @brief Console class
    /// @namespace cae
    ///
    class Console
    {
        public:
            Console() = default;
            ~Console() { stop(); }


            Console(const Console &) = delete;
            Console &operator=(const Console &) = delete;
            Console(const Console &&) = delete;
            Console &operator=(const Console &&) = delete;

            void create();
            void startInput();
            void stop();

            // --- window sync ---
            void attachTo(const NativeWindowHandle& handle);
            void syncWith(const NativeWindowHandle& handle);

        private:
            std::thread inputThread_;
            std::atomic<bool> running_{false};

            void consoleLoop();
            void handleCommand(const std::string& cmd);

#ifdef _WIN32
            HWND consoleHwnd_ = nullptr;
#endif
    }; // class Console

} // namespace cae
