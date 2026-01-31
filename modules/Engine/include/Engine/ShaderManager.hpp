///
/// @file ShaderManager.hpp
/// @brief This file contains the ShaderManager class declaration
/// @namespace cae::eng
///

#pragma once

#include "Interfaces/Shader/IShaderCompiler.hpp"

#include <memory>

namespace cae::eng
{
    ///
    /// @class ShaderManager
    /// @brief Class for managing shaders
    /// @namespace cae::eng
    ///
    class ShaderManager
    {
        public:
            explicit ShaderManager(
                const std::function<std::shared_ptr<IShaderCompiler>()> &shaderCompilerFactory)
            {
                if (shaderCompilerFactory)
                {
                    m_compiler = shaderCompilerFactory();
                }
            }
            ~ShaderManager() = default;

            ShaderManager(const ShaderManager &) = delete;
            ShaderManager &operator=(const ShaderManager &) = delete;
            ShaderManager(ShaderManager &&) = delete;
            ShaderManager &operator=(ShaderManager &&) = delete;

            std::unordered_map<ShaderID, ShaderIRModule> build(const std::vector<ShaderSourceDesc> &sources,
                                                               const ShaderSourceType targetIR) const
            {
                std::unordered_map<ShaderID, ShaderIRModule> out;

                for (const auto &src : sources)
                {
                    out[src.id] = m_compiler->compile(src);;
                }

                return out;
            }

            template <std::ranges::input_range R> void optimizeAll(const ShaderSourceType irType, R &&modules) const
            {
                if (auto it = m_irs.find(irType); it != m_irs.end())
                {
                    std::vector<ShaderIRModule *> ptrs;
                    for (auto &m : modules)
                    {
                        ptrs.push_back(&m);
                    }

                    std::vector<ShaderIRModule> tmp;
                    tmp.reserve(ptrs.size());
                    for (auto *p : ptrs)
                    {
                        tmp.push_back(*p);
                    }

                    it->second->optimize(tmp);

                    for (size_t i = 0; i < ptrs.size(); ++i)
                    {
                        *ptrs[i] = std::move(tmp[i]);
                    }
                }
            }

        private:

            std::shared_ptr<IShaderCompiler> m_compiler;

    }; // class ShaderManager

} // namespace cae::eng
