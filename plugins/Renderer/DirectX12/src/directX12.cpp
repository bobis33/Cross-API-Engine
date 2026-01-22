#include "DirectX12/DirectX12.hpp"

void cae::DirectX12::initialize(const NativeWindowHandle &window, const Color &clearColor)
{
    m_clearColor = clearColor;

    // Device
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));

    // Command queue
    D3D12_COMMAND_QUEUE_DESC cqDesc{};
    m_device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_commandQueue));

    // Swapchain
    DXGI_SWAP_CHAIN_DESC1 scDesc{};
    scDesc.BufferCount = FrameCount;
    scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
    factory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),
        static_cast<HWND>(window.display),
        &scDesc,
        nullptr,
        nullptr,
        &swapChain
    );

    swapChain.As(&m_swapChain);
}

void cae::DirectX12::draw(const WindowSize &, const ShaderID &, glm::mat4)
{
    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator.Get(), nullptr);

    // clear, barriers, etc (à compléter)
    m_commandList->Close();

    ID3D12CommandList* lists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(1, lists);

    m_swapChain->Present(m_vsync ? 1 : 0, 0);
}

