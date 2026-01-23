///
/// @file DirectX11.hpp
/// @brief This file contains the DirectX11 class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/ARenderer.hpp"

#include <glm/glm.hpp>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

namespace cae
{

    struct PipelineDX11
    {
        Microsoft::WRL::ComPtr<ID3D11VertexShader>   vs;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>    ps;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>    inputLayout;
    };

    ///
    /// @class DirectX11
    /// @brief Class for the DirectX11 plugin
    /// @namespace cae
    ///
    class DirectX11 final : public ARenderer
    {

        public:
            DirectX11() = default;
            ~DirectX11() override = default;

            DirectX11(const DirectX11 &) = delete;
            DirectX11 &operator=(const DirectX11 &) = delete;
            DirectX11(DirectX11 &&) = delete;
            DirectX11 &operator=(DirectX11 &&) = delete;

            [[nodiscard]] std::string getName() const override { return "DirectX11"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::RENDERER; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::WINDOWS; }

            void setVSyncEnabled(const bool enabled) override { m_vsync = enabled; }
            void setClearColor(const Color &color) override { m_clearColor = color; }

            [[nodiscard]] bool isVSyncEnabled() const override { return m_vsync; }

            void initialize(const NativeWindowHandle &nativeWindowHandle, const Color &clearColor) override;
            void createPipeline(const ShaderID &id, const ShaderIRModule &vertex,
                                const ShaderIRModule &fragment) override;
            void draw(const WindowSize &windowSize, const ShaderID &shaderId, glm::mat4 mvp) override;
            void createMesh(const std::vector<float> &vertices) override;

        private:
            Microsoft::WRL::ComPtr<ID3D11Device> m_device;
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
            Microsoft::WRL::ComPtr<IDXGISwapChain>  m_swapChain;
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
            Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
            // constant buffer MVP
            Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

            bool  m_vsync = true;
            Color m_clearColor{};

            std::unordered_map<ShaderID, PipelineDX11> m_pipelines;

    }; // class DirectX11

} // namespace cae
