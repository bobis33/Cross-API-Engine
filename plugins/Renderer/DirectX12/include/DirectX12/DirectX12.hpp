///
/// @file DirectX12.hpp
/// @brief This file contains the DirectX12 class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Renderer/ARenderer.hpp"

#include <glm/glm.hpp>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

namespace cae
{

    ///
    /// @class DirectX12
    /// @brief Class for the DirectX12 plugin
    /// @namespace cae
    ///
    class DirectX12 final : public ARenderer
    {

        public:
            DirectX12() = default;
            ~DirectX12() override = default;

            DirectX12(const DirectX12 &) = delete;
            DirectX12 &operator=(const DirectX12 &) = delete;
            DirectX12(DirectX12 &&) = delete;
            DirectX12 &operator=(DirectX12 &&) = delete;

            [[nodiscard]] std::string getName() const override { return "DirectX12"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::RENDERER; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::WINDOWS; }

            void setVSyncEnabled(bool enabled) override {}
            void setClearColor(const Color &color) override {}

            [[nodiscard]] bool isVSyncEnabled() const override { return false; }

            void initialize(const NativeWindowHandle &nativeWindowHandle, const Color &clearColor) override;
            void createPipeline(const ShaderID &id, const ShaderIRModule &vertex,
                                const ShaderIRModule &fragment) override
            {
            }
            void draw(const WindowSize &windowSize, const ShaderID &shaderId, glm::mat4 mvp) override;
            void createMesh(const std::vector<float> &vertices) override {}

        private:
            static constexpr uint32_t FrameCount = 2;

            // Core
            Microsoft::WRL::ComPtr<ID3D12Device>        m_device;
            Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
            Microsoft::WRL::ComPtr<IDXGISwapChain3>    m_swapChain;

            // Frame resources
            Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
            Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

            // RTV
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
            UINT m_rtvDescriptorSize = 0;

            // Sync
            Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
            UINT64 m_fenceValue = 0;
            HANDLE m_fenceEvent = nullptr;

            bool  m_vsync = true;
            Color m_clearColor{};

    }; // class DirectX12

} // namespace cae