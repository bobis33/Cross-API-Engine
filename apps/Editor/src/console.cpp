#include "CAE/Console.hpp"

namespace cae
{

    void Console::create()
    {
#ifdef _WIN32
        if (!GetConsoleWindow())
        {
            AllocConsole();
        }

        consoleHwnd_ = GetConsoleWindow();

        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);

        std::ios::sync_with_stdio(true);
        SetConsoleTitleA("CAE Console");

        utl::Logger::addSink([](const std::string& msg, utl::LogLevel) {
            std::cout << msg << std::endl;
        });
#endif
    }

    void Console::startInput()
    {
        running_ = true;
        inputThread_ = std::thread(&Console::consoleLoop, this);
    }

    void Console::stop()
    {
        running_ = false;
        if (inputThread_.joinable())
            inputThread_.join();
    }

    void Console::consoleLoop()
    {
        std::string line;
        while (running_)
        {
            std::cout << "> " << std::flush;
            if (!std::getline(std::cin, line))
                break;

            handleCommand(line);
        }
    }

    void Console::handleCommand(const std::string& cmd)
    {
        if (cmd == "exit")
            utl::Logger::log("Exit command received", utl::LogLevel::INFO);
        else
            utl::Logger::log("Unknown command: " + cmd, utl::LogLevel::WARNING);
    }

    void Console::attachTo(const NativeWindowHandle& handle)
    {
#ifdef _WIN32
        HWND app = static_cast<HWND>(handle.window);
        if (!app || !consoleHwnd_) return;

        RECT r;
        GetWindowRect(app, &r);

        MoveWindow(
            consoleHwnd_,
            r.right,
            r.top,
            500,
            r.bottom - r.top,
            TRUE
        );
#endif
    }

    void Console::syncWith(const NativeWindowHandle& handle)
    {
#ifdef _WIN32
        HWND app = static_cast<HWND>(handle.window);
        if (!app || !consoleHwnd_) return;

        ShowWindow(consoleHwnd_, IsIconic(app) ? SW_MINIMIZE : SW_SHOW);
#endif
    }

} // namespace cae
