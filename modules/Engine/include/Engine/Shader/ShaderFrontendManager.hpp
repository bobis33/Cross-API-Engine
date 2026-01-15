///
/// @file ShaderFrontendManager.hpp
/// @brief This file contains the ShaderFrontendManager class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Shader/IShaderFrontend.hpp"

#include <memory>
#include <unordered_map>

namespace cae
{
    ///
    /// @class ShaderManager
    /// @brief Class for managing shader frontends and sources
    /// @namespace cae
    ///
    class ShaderManager
    {
        public:
            ShaderManager() = default;
            ~ShaderManager() = default;

            ShaderManager(const ShaderManager &) = delete;
            ShaderManager &operator=(const ShaderManager &) = delete;
            ShaderManager(ShaderManager &&) = delete;
            ShaderManager &operator=(ShaderManager &&) = delete;

            void registerFrontend(const std::shared_ptr<IShaderFrontend> &frontend)
            {
                m_frontends[frontend->sourceType()] = frontend;
            }
            void addSource(const ShaderSourceDesc &desc) { m_sources[desc.id] = desc; }
            bool compileAll()
            {
                for (auto &[id, src] : m_sources)
                {
                    auto it = m_frontends.find(src.type);
                    if (it == m_frontends.end())
                    {
                        throw std::runtime_error("No frontend for this shader type");
                    }

                    m_ir[id] = it->second->compile(src);
                }
                return true;
            }
            const ShaderIRModule &getIR(const ShaderID &id) const
            {
                const auto it = m_ir.find(id);
                if (it == m_ir.end())
                {
                    throw std::runtime_error("Shader IR not found");
                }

                return it->second;
            }

            const std::unordered_map<ShaderID, ShaderIRModule> &getAllIR() const { return m_ir; }

        private:
            std::unordered_map<ShaderSourceType, std::shared_ptr<IShaderFrontend>> m_frontends;
            std::unordered_map<ShaderID, ShaderSourceDesc> m_sources;
            std::unordered_map<ShaderID, ShaderIRModule> m_ir;

    }; // class ShaderManager

} // namespace cae
