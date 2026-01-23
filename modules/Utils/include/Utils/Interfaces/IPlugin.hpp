///
/// @file IPlugin.hpp
/// @brief This file contains the plugin interface
/// @namespace cae::utl
///

#pragma once

#include <cstdint>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT
#endif

namespace cae::utl
{

    enum class PluginType : std::uint8_t
    {
        AUDIO = 0,
        MODEL = 1,
        NETWORK = 2,
        PHYSIC = 3,
        RENDERER = 4,
        SHADER_FRONTEND = 5,
        SHADER_IR = 6,
        WINDOW = 7,
        UNDEFINED = 255,
    };

    enum class PluginPlatform : std::uint8_t
    {
        ALL = 0,
        ANDROID = 1,
        LINUX = 2,
        MACOSX = 3,
        WEB = 4,
        WINDOWS = 5,
        UNDEFINED = 255
    };

    ///
    /// @interface IPlugin
    /// @brief Interface for plugins
    /// @namespace cae::utl
    ///
    class IPlugin
    {

        public:
            virtual ~IPlugin() = default;

            ///
            /// @return The name of the plugin
            /// @brief Get the name of the plugin
            ///
            [[nodiscard]] virtual std::string getName() const = 0;

            ///
            /// @return The type of the plugin
            /// @brief Get the type of the plugin
            ///
            [[nodiscard]] virtual PluginType getType() const = 0;

            ///
            /// @return The handled platform of the plugin
            /// @brief Get the handled platform of the plugin
            ///
            [[nodiscard]] virtual PluginPlatform getPlatform() const = 0;

    }; // interface IPlugin

} // namespace cae::utl
