///
/// @file Path.hpp
/// @brief This file contains Path resolution utilities
/// @namespace utl
///

#pragma once

#ifdef _WIN32
#include <windows.h>
#elifdef __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif

#include <filesystem>
#include <string>

namespace utl
{

    namespace fs = std::filesystem;

    ///
    /// @class Path
    /// @brief Class for path resolution utilities
    /// @namespace utl
    ///
    class Path
    {

        public:
            static fs::path normalize(const fs::path &path)
            {
                try
                {
                    return fs::weakly_canonical(path);
                }
                catch (...)
                {
                    return fs::absolute(path);
                }
            }

            static bool existsFile(const fs::path &path) { return fs::exists(path) && fs::is_regular_file(path); }

            static bool existsDir(const fs::path &path) { return fs::exists(path) && fs::is_directory(path); }

            static fs::path parentDir(const fs::path &path) { return normalize(path).parent_path(); }

            template <typename... Paths> static fs::path join(const Paths &...paths)
            {
                return normalize((fs::path(paths) / ...));
            }

            fs::path executableDir()
            {
#if defined(_WIN32)
                char buffer[MAX_PATH];
                GetModuleFileNameA(NULL, buffer, MAX_PATH);
                return fs::path(buffer).parent_path();
#elif defined(__APPLE__)
                char buffer[1024];
                uint32_t size = sizeof(buffer);
                if (_NSGetExecutablePath(buffer, &size) == 0)
                {
                    return fs::path(buffer).parent_path();
                }
                return fs::current_path();
#else
                char buffer[1024];
                if (const ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer)); count != -1)
                {
                    return fs::path(std::string(buffer, count)).parent_path();
                }
                return fs::current_path();
#endif
            }

            fs::path resolveRelativeToExe(const fs::path &relativePath)
            {
                return normalize(executableDir() / relativePath);
            }

    }; // class Path

} // namespace utl
