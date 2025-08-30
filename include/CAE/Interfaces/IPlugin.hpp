///
/// @file IPlugin.hpp
/// @brief This file contains the IPlugin interface
/// @namespace cae
///

#pragma once

namespace cae
{

    enum class PluginType : uint8_t {
        RENDERER = 0,
        WINDOW = 1,
        UNKNOWN = 255
    };

    ///
    /// @interface IPlugin
    /// @brief Interface for plugins
    /// @namespace cae
    ///
    class IPlugin {

        public:
            virtual ~IPlugin() = default;
        [[nodiscard]] virtual const char* getName() const = 0;
        [[nodiscard]] virtual PluginType getType() const = 0;

    }; // interface IPlugin

} // namespace cae
