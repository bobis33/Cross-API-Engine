# Shader Plugins

`frontend` -> `IR` -> `Renderer`

---

## Supported Languages & Target APIs

| Language | Compatibility   |
|:---------|:----------------|
| GLSL     | OpenGL - Vulkan |
| HLSL     | DirectX         |
| MSL      | Metal           |
| WGSL     | WebGPU          |

---

## Plugin Types

| Plugin Type | Description                                                                            |
|:------------|:---------------------------------------------------------------------------------------|
| Frontend    | compile human-readable shader source into SPIR-V                                       |
| IR          | post-process, optimize, or analyze SPIR-V modules, and transform to the target Backend |
