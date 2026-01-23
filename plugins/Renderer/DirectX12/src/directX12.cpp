#include "DirectX12/DirectX12.hpp"

#include <stdexcept>

void cae::DirectX12::initialize(const NativeWindowHandle &nativeWindowHandle, const Color &clearColor)
{
    m_clearColor = clearColor;
    UINT createFactoryFlags = 0;

#if defined(CAE_DEBUG)
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

    Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
#endif

    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
    HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create DXGI Factory");
    }

    // --------------------
    // Device
    // --------------------
    hr = D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&m_device)
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D3D12 device");
    }

    // --------------------
    // Command queue
    // --------------------
    D3D12_COMMAND_QUEUE_DESC cqDesc{};
    cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    hr = m_device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_commandQueue));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create command queue");
    }

    // --------------------
    // Command allocator
    // --------------------
    hr = m_device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&m_commandAllocator)
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create command allocator");
    }

    // --------------------
    // Command list
    // --------------------
    m_device->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_commandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(&m_commandList)
    );
    m_commandList->Close();

    // --------------------
    // Swapchain
    // --------------------
    if (nativeWindowHandle.window == nullptr)
    {
        throw std::runtime_error("Window handle is null");
    }
    DXGI_SWAP_CHAIN_DESC1 scDesc{};
    RECT rc;
    GetClientRect(static_cast<HWND>(nativeWindowHandle.window), &rc);
    scDesc.BufferCount = FrameCount;
    scDesc.Width = rc.right - rc.left;
    scDesc.Height = rc.bottom - rc.top;
    scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGIFactory4> factory2;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory2));

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;

    hr = factory2->CreateSwapChainForHwnd(
        m_commandQueue.Get(),
        static_cast<HWND>(nativeWindowHandle.window),
        &scDesc,
        nullptr,
        nullptr,
        &swapChain
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create swapchain");
    }

    hr = swapChain.As(&m_swapChain);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create swapchain");
    }

    // --------------------
    // RTV heap
    // --------------------
    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc{};
    rtvDesc.NumDescriptors = FrameCount;
    rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    m_device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&m_rtvHeap));
    m_rtvDescriptorSize =
        m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // --------------------
    // Create RTVs
    // --------------------
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < FrameCount; ++i)
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to get swapchain buffer");
        }

        m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);
    }

    // --------------------
    // Fence
    // --------------------
    m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    m_fenceValue = 1;
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}


void cae::DirectX12::draw(const WindowSize &, const ShaderID &, glm::mat4)
{
    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator.Get(), nullptr);

    m_commandList->Close();

    ID3D12CommandList* lists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(1, lists);

    m_swapChain->Present(m_vsync ? 1 : 0, 0);
}

