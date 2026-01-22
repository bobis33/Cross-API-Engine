///
/// @file Path.hpp
/// @brief This file contains Path resolution utilities
/// @namespace cae::utl
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

namespace cae::utl
{

    namespace fs = std::filesystem;

    ///
    /// @class Path
    /// @brief Class for path resolution utilities
    /// @namespace cae::utl
    ///
    class Path
    {

        public:
            Path() = default;
            ~Path() = default;

            Path(const Path &) = delete;
            Path &operator=(const Path &) = delete;
            Path(Path &&) = delete;
            Path &operator=(Path &&) = delete;

            /// @param path Path to be normalized
            /// @return Normalized path
            /// @brief Normalize a path (resolve symlinks, relative paths, etc.)
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

            ///
            /// @param path Path to be checked
            /// @return True if the file exists
            /// @brief Check if a file exists
            ///
            static bool existsFile(const fs::path &path) { return fs::exists(path) && fs::is_regular_file(path); }

            ///
            /// @param path Path to be checked
            /// @return True if the directory exists
            /// @brief Check if a directory exists
            ///
            static bool existsDir(const fs::path &path) { return fs::exists(path) && fs::is_directory(path); }

            ///
            /// @param path Path to get the parent directory of
            /// @return Parent directory of the path
            /// @brief Get the parent directory of a path
            ///
            static fs::path parentDir(const fs::path &path) { return normalize(path).parent_path(); }

            ///
            /// @tparam Paths Variadic template for paths
            /// @param paths Paths to be joined
            /// @return Joined path
            /// @brief Join multiple paths
            ///
            template <typename... Paths> static fs::path join(const Paths &...paths)
            {
                return normalize((fs::path(paths) / ...));
            }

            ///
            /// @return Directory of the executable
            /// @brief Get the directory of the executable
            ///
            static fs::path executableDir()
            {
#ifdef _WIN32
                char buffer[MAX_PATH];
                GetModuleFileNameA(NULL, buffer, MAX_PATH);
                return fs::path(buffer).parent_path();
#elifdef __APPLE__
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

            ///
            /// @param relativePath Relative path to be resolved
            /// @return Resolved path relative to the executable directory
            /// @brief Resolve a relative path to the executable directory
            ///
            static fs::path resolveRelativeToExe(const fs::path &relativePath)
            {
                return normalize(executableDir() / relativePath);
            }

            ///
            /// @param relativePath Relative path to be resolved
            /// @return Resolved path relative to the user cwd
            /// @brief
            ///
            static fs::path resolveRelativeToCwd(const fs::path &relativePath)
            {
                return normalize(fs::current_path() / relativePath);
            }

    }; // class Path

} // namespace cae::utl
