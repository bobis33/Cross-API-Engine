///
/// @file ShaderManager.hpp
/// @brief This file contains the ShaderManager class declaration
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
    /// @class ShaderManager
    /// @brief Class for managing shaders
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

            void registerFrontend(const std::shared_ptr<IShaderFrontend> &f) { m_frontends[f->sourceType()] = f; }
            void registerIR(const std::shared_ptr<IShaderIR> &ir) { m_irs[ir->irType()] = ir; }

            std::unordered_map<ShaderID, ShaderIRModule> build(const std::vector<ShaderSourceDesc> &sources,
                                                               const ShaderSourceType targetIR) const
            {
                std::unordered_map<ShaderID, ShaderIRModule> out;

                const auto irProcessor = m_irs.at(targetIR);
                for (const auto &src : sources)
                {
                    const auto f = m_frontends.at(src.type);
                    ShaderIRModule ir = f->compile(src);
                    const ShaderIRModule final = irProcessor->process(ir);
                    out[src.id] = final;
                }

                return out;
            }

            template <std::ranges::input_range R> void optimizeAll(ShaderSourceType irType, R &&modules) const
            {
                if (auto it = m_irs.find(irType); it != m_irs.end())
                {
                    std::vector<ShaderIRModule *> ptrs;
                    for (auto &m : modules)
                        ptrs.push_back(&m);

                    std::vector<ShaderIRModule> tmp;
                    tmp.reserve(ptrs.size());
                    for (auto *p : ptrs)
                        tmp.push_back(*p);

                    it->second->optimize(tmp);

                    // recopier vers les originaux si besoin
                    for (size_t i = 0; i < ptrs.size(); ++i)
                        *ptrs[i] = std::move(tmp[i]);
                }
            }

        private:
            std::unordered_map<ShaderSourceType, std::shared_ptr<IShaderFrontend>> m_frontends;
            std::unordered_map<ShaderSourceType, std::shared_ptr<IShaderIR>> m_irs;

    }; // class ShaderManager

} // namespace cae
