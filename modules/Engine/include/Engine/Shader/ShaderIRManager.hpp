///
/// @file ShaderIRManager.hpp
/// @brief This file contains the ShaderIRManager class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/IShaderIR.hpp"

#include <memory>
#include <ranges>
#include <unordered_map>

namespace cae
{
    ///
    /// @class ShaderIRManager
    /// @brief Class for managing shader IR plugins
    /// @namespace cae
    ///
    class ShaderIRManager
    {
        public:
            ShaderIRManager() = default;
            ~ShaderIRManager() = default;

            ShaderIRManager(const ShaderIRManager &) = delete;
            ShaderIRManager &operator=(const ShaderIRManager &) = delete;
            ShaderIRManager(ShaderIRManager &&) = delete;
            ShaderIRManager &operator=(ShaderIRManager &&) = delete;

            void registerPlugin(const std::shared_ptr<IShaderIR> &plugin) { m_plugins[plugin->irType()] = plugin; }

            ShaderIRModule process(const ShaderIRModule &module)
            {
                const auto it = m_plugins.find(moduleType(module));
                if (it == m_plugins.end())
                {
                    throw std::runtime_error("No IR plugin for this module type");
                }

                return it->second->process(module);
            }

            void optimizeAll(std::vector<ShaderIRModule> &modules)
            {
                for (const auto &plugin : m_plugins | std::views::values)
                {
                    plugin->optimize(modules);
                }
            }

        private:
            std::unordered_map<ShaderSourceType, std::shared_ptr<IShaderIR>> m_plugins;

            static ShaderSourceType moduleType(const ShaderIRModule &m) { return ShaderSourceType::SPIRV; }

    }; // class ShaderIRManager

} // namespace cae
