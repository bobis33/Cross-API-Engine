#include "SPIR-V/SPIR-V.hpp"

#include "Utils/Logger.hpp"

void cae::SPIRV::addShader(const ShaderModuleDesc& pipelineDesc)
{
    ShaderData data;
    data.type = ShaderSourceType::GLSL;
    data.source = pipelineDesc.source;
    data.stage = pipelineDesc.stage;
    data.entryPoint = "main";
    m_shaders[pipelineDesc.id] = std::move(data);
}

bool cae::SPIRV::compileAll()
{
    bool allCompiled = true;
    for (auto& [name, shader] : m_shaders) {
        try {
            shader.spirv = compileGLSLtoSPIRV(shader.source, shader.stage);
            m_compiled[name] = true;
        } catch (const std::exception& e) {
            m_compiled[name] = false;
            allCompiled = false;
            utl::Logger::log("Shader compilation failed [" + name + "]: " + e.what(), utl::LogLevel::INFO);
        }
    }
    return allCompiled;
}

const ShaderData& cae::SPIRV::getShader(const std::string& name) const
{
    auto it = m_shaders.find(name);
    if (it == m_shaders.end()) {
        throw std::runtime_error("Shader not found: " + name);
    }
    return it->second;
}

bool cae::SPIRV::isCompiled(const std::string& name) const
{
    auto it = m_compiled.find(name);
    return it != m_compiled.end() && it->second;
}
