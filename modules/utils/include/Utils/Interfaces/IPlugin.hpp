///
/// @file IPlugin.hpp
/// @brief This file contains the plugin interface
/// @namespace utl
///

#pragma once

#include <cstdint>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT
#endif

namespace utl
{

    enum class PluginType : uint8_t
    {
        AUDIO = 0,
        NETWORK = 1,
        RENDERER = 2,
        WINDOW = 3,
        UNDEFINED = 255,
    };

    enum PluginPlatform : uint8_t
    {
        LINUX = 0,
        MACOSX = 1,
        WINDOWS = 2,
        ALL = 255
    };

    ///
    /// @interface IPlugin
    /// @brief Interface for plugins
    /// @namespace utl
    ///
    class IPlugin
    {

        public:
            virtual ~IPlugin() = default;

            [[nodiscard]] virtual std::string getName() const = 0;
            [[nodiscard]] virtual PluginType getType() const = 0;
            [[nodiscard]] virtual PluginPlatform getPlatform() const = 0;

    }; // interface IPlugin
} // namespace utl
