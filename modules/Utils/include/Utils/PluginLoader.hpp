///
/// @file PluginLoader.hpp
/// @brief Modern, cross-platform plugin loader
/// @namespace utl
///

#pragma once

#include <filesystem>
#include <mutex>

#ifdef _WIN32
#include <windows.h>
#define PLUGINS_EXTENSION ".dll"
#define PLUGINS_PREFIX ""
#else
#include <dlfcn.h>
#ifdef __APPLE__
#define PLUGINS_EXTENSION ".dylib"
#elif __linux__
#define PLUGINS_EXTENSION ".so"
#endif
#define PLUGINS_PREFIX "lib"
#endif

namespace utl
{

    using LibHandle =
#ifdef _WIN32
        HMODULE;
#else
        void *;
#endif

    ///
    /// @struct SharedLib
    /// @brief RAII wrapper for shared libraries
    /// @namespace utl
    ///
    struct SharedLib
    {
            LibHandle handle = nullptr;

            explicit SharedLib(const LibHandle h = nullptr) : handle(h) {}
            ~SharedLib() { close(); }

            SharedLib(const SharedLib &) = delete;
            SharedLib &operator=(const SharedLib &) = delete;
            SharedLib(SharedLib &&other) noexcept : handle(other.handle) { other.handle = nullptr; }
            SharedLib &operator=(SharedLib &&other) noexcept
            {
                if (this != &other)
                {
                    close();
                    handle = other.handle;
                    other.handle = nullptr;
                }
                return *this;
            }

            void close()
            {
                if (handle == nullptr)
                {
                    return;
                }
#ifdef _WIN32
                FreeLibrary(handle);
#else
                dlclose(handle);
#endif
                handle = nullptr;
            }

            explicit operator bool() const { return handle != nullptr; }
    };

    using EntryPointFn = IPlugin *(*)();

    ///
    /// @class PluginLoader
    /// @brief Modern, type-safe plugin loader
    /// @namespace utl
    ///
    class PluginLoader
    {
        public:
            PluginLoader() = default;
            ~PluginLoader() = default;

            PluginLoader(const PluginLoader &) = delete;
            PluginLoader &operator=(const PluginLoader &) = delete;
            PluginLoader(PluginLoader &&) = delete;
            PluginLoader &operator=(PluginLoader &&) = delete;

            ///
            /// @tparam T Expected plugin interface (must derive from IPlugin)
            /// @param path Path to the dynamic library
            /// @param pluginPrefix Expected prefix for plugin filenames
            /// @return shared_ptr<T> instance
            /// @brief Load a plugin of type T
            ///
            template <std::derived_from<IPlugin> T>
            std::shared_ptr<T> loadPlugin(const std::string &path, const std::string_view &pluginPrefix = "")
            {
                std::scoped_lock lock(m_mutex);

                try
                {
                    validatePluginPath(path, PLUGINS_PREFIX + std::string(pluginPrefix));

                    if (m_plugins.contains(path))
                    {
                        return nullptr;
                    }

                    SharedLib lib = loadLibrary(path);
                    const EntryPointFn entry = getEntryPoint(lib, path);

                    std::unique_ptr<IPlugin> plugin(entry());
                    if (!plugin)
                    {
                        throw std::runtime_error("EntryPoint returned null");
                    }

                    T *typed = dynamic_cast<T *>(plugin.get());
                    if (!typed)
                    {
                        throw std::runtime_error("Plugin type mismatch");
                    }

                    auto [it, inserted] = m_plugins.emplace(path, std::move(plugin));
                    if (!inserted)
                    {
                        throw std::runtime_error("Failed to store plugin");
                    }

                    m_handles[path] = std::move(lib);

                    std::shared_ptr<IPlugin> baseShared(it->second.get(), [](IPlugin *) {});
                    return std::shared_ptr<T>(baseShared, typed);
                }
                catch (const std::exception &e)
                {
                    throw std::runtime_error(std::string("Unknown exception: ") + e.what());
                    return nullptr;
                }
            }

        private:
            std::mutex m_mutex;
            std::unordered_map<std::string, SharedLib> m_handles;
            std::unordered_map<std::string, std::unique_ptr<IPlugin>> m_plugins;

            ///
            /// @param path Path to the dynamic library
            /// @return Loaded SharedLib
            /// @brief Load a shared library
            ///
            static SharedLib loadLibrary(const std::string &path)
            {
                const LibHandle handle =
#ifdef _WIN32
                    LoadLibraryA(path.c_str());
#else
                    dlopen(path.c_str(), RTLD_LAZY);
#endif
                if (handle == nullptr)
                {
                    const std::string dlErrorMsg =
#ifdef _WIN32
                        "LoadLibraryA failed";
#else
                        dlerror();
#endif
                    throw std::runtime_error("Cannot load library: " + dlErrorMsg + "\nWith path: " + path);
                }
                return SharedLib(handle);
            }

            ///
            /// @param lib Loaded SharedLib
            /// @param path Path to the dynamic library
            /// @return EntryPoint function pointer
            ///
            static EntryPointFn getEntryPoint(SharedLib &lib, const std::string &path)
            {
                const auto entry =
#ifdef _WIN32
                    reinterpret_cast<EntryPointFn>(GetProcAddress(lib.handle, "entryPoint"));
#else
                    reinterpret_cast<EntryPointFn>(dlsym(lib.handle, "entryPoint"));
#endif
                if (entry == nullptr)
                {
                    throw std::runtime_error("EntryPoint not found in plugin: " + path);
                }
                return entry;
            }

            ///
            /// @param path Path to the dynamic library
            /// @param pluginPrefix Expected prefix for plugin filenames
            /// @brief Validate the plugin path
            ///
            static void validatePluginPath(const std::filesystem::path &path, const std::string_view &pluginPrefix)
            {
                const std::string filename = path.filename().string();

                if (path.extension() != PLUGINS_EXTENSION)
                {
                    throw std::runtime_error("Invalid plugin extension: " + filename +
                                             " (expected " PLUGINS_EXTENSION ")");
                }

                if (!filename.starts_with(pluginPrefix))
                {
                    throw std::runtime_error("Invalid plugin name: " + filename + " (plugins must start with '" +
                                             std::string(pluginPrefix) + "')");
                }
            }

    }; // class PluginLoader

} // namespace utl
