///
/// @file ShaderManager.hpp
/// @brief This file contains the ShaderManager class declaration
/// @namespace cae::eng
///

#pragma once

#include "Interfaces/Shader/IR/IShaderIR.hpp"

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
                const std::vector<std::function<std::shared_ptr<IShaderFrontend>()>> &shaderFrontendFactories,
                const std::function<std::shared_ptr<IShaderIR>()> &shaderIRFactory = nullptr)
            {
                for (const auto &factory : shaderFrontendFactories)
                {
                    auto frontend = factory();
                    registerFrontend(frontend);
                }
                if (shaderIRFactory)
                {
                    registerIR(shaderIRFactory());
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
                auto it = m_irs.find(targetIR);
                if (it == m_irs.end())
                {
                    throw std::runtime_error(
                        "No ShaderIR registered for specified target type"
                    );
                }
                const auto& irProcessor = it->second;
                for (const auto &src : sources)
                {
                    const auto f = m_frontends.at(src.type);
                    ShaderIRModule ir = f->compile(src);
                    const ShaderIRModule final = irProcessor->process(ir);
                    out[src.id] = final;
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
            void registerFrontend(const std::shared_ptr<IShaderFrontend> &f) { m_frontends[f->sourceType()] = f; }
            void registerIR(const std::shared_ptr<IShaderIR> &ir) { m_irs[ir->irType()] = ir; }

            std::unordered_map<ShaderSourceType, std::shared_ptr<IShaderFrontend>> m_frontends;
            std::unordered_map<ShaderSourceType, std::shared_ptr<IShaderIR>> m_irs;

    }; // class ShaderManager

} // namespace cae::eng
