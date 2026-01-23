#include "HLSL/HLSL.hpp"
#include "Interfaces/Shader/Frontend/IShaderFrontend.hpp"

#include <combaseapi.h>
#include <stdexcept>
#include <string>
#include <vector>

#include <dxcapi.h>
#include <atlbase.h>

cae::ShaderIRModule cae::HLSL::compile(const ShaderSourceDesc & desc)
{
    ShaderIRModule ir;
    ir.id = desc.id;
    ir.stage = desc.stage;
    ir.entryPoint = "main";

    bool dx12 = true;
    std::string targetProfile = stageToTargetProfile(desc.stage, dx12);

    CComPtr<IDxcCompiler> compiler;
    CComPtr<IDxcLibrary> library;
    CComPtr<IDxcBlobEncoding> sourceBlob;
    CComPtr<IDxcOperationResult> result;

    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
    DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));

    library->CreateBlobWithEncodingFromPinned(
        desc.source.c_str(),
        static_cast<UINT32>(desc.source.size()),
        CP_UTF8,
        &sourceBlob
    );

    CComPtr<IDxcBlobEncoding> argsBlob;

    compiler->Compile(
        sourceBlob,
        L"shader.hlsl",
        L"main",
        std::wstring(targetProfile.begin(), targetProfile.end()).c_str(),
        nullptr, 0, nullptr, 0, nullptr,
        &result
    );

    HRESULT hr;
    result->GetStatus(&hr);
    if (FAILED(hr)) {
        CComPtr<IDxcBlobEncoding> errors;
        result->GetErrorBuffer(&errors);
        std::string errMsg(static_cast<const char*>(errors->GetBufferPointer()), errors->GetBufferSize());
        throw std::runtime_error("HLSL compile failed:\n" + errMsg);
    }

    CComPtr<IDxcBlob> shaderBlob;
    result->GetResult(&shaderBlob);

    ir.dxil.assign(
        static_cast<const uint8_t*>(shaderBlob->GetBufferPointer()),
        static_cast<const uint8_t*>(shaderBlob->GetBufferPointer()) + shaderBlob->GetBufferSize()
    );

    return ir;
}

std::string cae::HLSL::stageToTargetProfile(ShaderStage stage, bool dx12)
{
    switch(stage)
    {
        case ShaderStage::VERTEX:   return dx12 ? "vs_6_0" : "vs_5_0";
        case ShaderStage::FRAGMENT: return dx12 ? "ps_6_0" : "ps_5_0";
        case ShaderStage::COMPUTE:  return dx12 ? "cs_6_0" : "cs_5_0";
        default: throw std::runtime_error("Unsupported ShaderStage for HLSL");
    }
}
