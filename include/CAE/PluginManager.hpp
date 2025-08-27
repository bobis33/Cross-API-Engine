///
/// @file PluginManager.hpp
/// @brief This file contains the PluginManager class declaration
/// @namespace CAE
///

#pragma once

#include <string>
#include <memory>
#include <ranges>
#include <unordered_map>

#include "Interfaces/IPlugin.hpp"


namespace CAE
{

    ///
    /// @class PluginManager
    /// @brief Class for managing plugins
    /// @namespace CAE
    ///
    class PluginManager
    {

        public:
            PluginManager() = default;
            ~PluginManager();

            PluginManager(const PluginManager &) = delete;
            PluginManager &operator=(const PluginManager &) = delete;
            PluginManager(PluginManager &&) = delete;
            PluginManager &operator=(PluginManager &&) = delete;

            IPlugin* loadPlugin(const std::string& path);
            void unloadPlugin(const std::string& path);

        private:
    #ifdef _WIN32
            using LibHandle = HMODULE;
    #else
            using LibHandle = void*;
    #endif
            std::unordered_map<std::string, LibHandle> m_handles;
            std::unordered_map<std::string, std::unique_ptr<IPlugin>> m_plugins;

    }; // class PluginManager

} // namespace CAE