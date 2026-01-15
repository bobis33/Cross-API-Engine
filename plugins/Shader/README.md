# Shader Plugins

`frontend` -> `IR` -> `Renderer`

---

## Supported Languages & Target APIs

| Language | Compatibility (renderer) | Compatibility (IR)                  |
|:---------|:-------------------------|:------------------------------------|
| GLSL     | OpenGL - Vulkan          | SPIR-V → HLSL/MSL/WGSL              |
| HLSL     | DirectX                  | DXC → DXIL / SPIR-V → GLSL/MSL/WGSL |
| MSL      | Metal                    | SPIR-V → MSL (SPIRV-Cross)          |
| WGSL     | WebGPU                   | SPIR-V → WGSL (via SPIRV-Cross)     |

---

## Plugin Types

| Plugin Type | Description                                                                            |
|:------------|:---------------------------------------------------------------------------------------|
| Frontend    | compile human-readable shader source into SPIR-V                                       |
| IR          | post-process, optimize, or analyze SPIR-V modules, and transform to the target Backend |
