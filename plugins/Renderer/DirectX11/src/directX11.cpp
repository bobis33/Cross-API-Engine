#include "DirectX11/DirectX11.hpp"

#include <stdexcept>

void cae::DirectX11::initialize(const NativeWindowHandle &window, const Color &clearColor)
{
    m_clearColor = clearColor;

    DXGI_SWAP_CHAIN_DESC scd{};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = static_cast<HWND>(window.display);
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_context
    );

    // Create RTV
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);
}

void cae::DirectX11::draw(
    const WindowSize &windowSize,
    const ShaderID &shaderId,
    glm::mat4 mvp)
{
    const FLOAT clear[4] = {
        m_clearColor.r,
        m_clearColor.g,
        m_clearColor.b,
        m_clearColor.a
    };

    // viewport
    D3D11_VIEWPORT vp{};
    vp.Width = static_cast<float>(windowSize.width);
    vp.Height = static_cast<float>(windowSize.height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &vp);

    m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), nullptr);
    m_context->ClearRenderTargetView(m_rtv.Get(), clear);

    const auto &pipeline = m_pipelines.at(shaderId);

    // shaders
    m_context->IASetInputLayout(pipeline.inputLayout.Get());
    m_context->VSSetShader(pipeline.vs.Get(), nullptr, 0);
    m_context->PSSetShader(pipeline.ps.Get(), nullptr, 0);

    // vertex buffer
    UINT stride = 6 * sizeof(float);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // update MVP
    D3D11_MAPPED_SUBRESOURCE mapped{};
    m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &mvp, sizeof(glm::mat4));
    m_context->Unmap(m_constantBuffer.Get(), 0);

    m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    // draw
    m_context->Draw(3, 0);

    m_swapChain->Present(m_vsync ? 1 : 0, 0);
}


void cae::DirectX11::createPipeline(
    const ShaderID &id,
    const ShaderIRModule &vertex,
    const ShaderIRModule &fragment)
{
    PipelineDX11 pipeline{};

    // Vertex shader
    HRESULT hr = m_device->CreateVertexShader(
        vertex.dxbc.data(),
        vertex.dxbc.size(),
        nullptr,
        &pipeline.vs
    );
    if (FAILED(hr))
        throw std::runtime_error("Failed to create DX11 vertex shader");

    // Pixel shader
    hr = m_device->CreatePixelShader(
        fragment.dxbc.data(),
        fragment.dxbc.size(),
        nullptr,
        &pipeline.ps
    );
    if (FAILED(hr))
        throw std::runtime_error("Failed to create DX11 pixel shader");

    // Input layout (position + color, comme ton OpenGL)
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
          D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = m_device->CreateInputLayout(
        layout,
        2,
        vertex.dxbc.data(),
        vertex.dxbc.size(),
        &pipeline.inputLayout
    );
    if (FAILED(hr))
        throw std::runtime_error("Failed to create input layout");

    m_pipelines[id] = pipeline;

    // Create constant buffer (MVP)
    if (!m_constantBuffer)
    {
        D3D11_BUFFER_DESC cbd{};
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.ByteWidth = sizeof(glm::mat4);

        m_device->CreateBuffer(&cbd, nullptr, &m_constantBuffer);
    }
}

void cae::DirectX11::createMesh(const std::vector<float> &vertices)
{
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(float));
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA init{};
    init.pSysMem = vertices.data();

    HRESULT hr = m_device->CreateBuffer(&bd, &init, &m_vertexBuffer);
    if (FAILED(hr))
        throw std::runtime_error("Failed to create vertex buffer");
}

