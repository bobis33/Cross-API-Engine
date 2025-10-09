[![CI](https://github.com/bobis33/Cross-API-Engine/actions/workflows/ci.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/ci.yml)
[![Gitleaks](https://github.com/bobis33/Cross-API-Engine/actions/workflows/gitleaks.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/gitleaks.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/bobis33/Cross-API-Engine/blob/main/LICENSE.md)

# Cross-API-Engine | Rendering Engine with multiple dynamic backends

Cross-API-Engine is a rendering engine designed to support multiple graphics APIs dynamically.
It allows developers to switch between different rendering backends such as OpenGL, Vulkan, DirectX at runtime.
It is useful to do benchmarks during development or to support multiple platforms with different graphics APIs.

```mermaid
flowchart LR
subgraph main
    subgraph App
        A[Engine]
        A -->|.so/.dylib/.dll| B[IAudio]
        A -->|.so/.dylib/.dll| C[INetwork]
        A -->|.so/.dylib/.dll| D[IRenderer]
        A -->|.so/.dylib/.dll| E[IWindow]
    end

    subgraph Plugins
        subgraph audio impl
            F[WASAPI]
            K[ALSA]
            L[CoreAudio]
        end
        subgraph network impl
            G[Winsock2]
            O[Berkeley Sockets]
        end
        subgraph renderer impl
            H[OpenGL]
            I[Vulkan]
        end
        subgraph window impl
            J[WIN32]
            M[WayLand]
            N[Cocoa]
            subgraph Input devices
                P[IInput]
                Q[KeyBoard]
                R[Mouse]
                S[Controller]
            end
        end
    end

    B -.-> F
    B -.-> K
    B -.-> L
    C -.-> G
    C -.-> O
    D -.-> H
    D -.-> I
    E -.-> P
    E -.-> J
    E -.-> M
    E -.-> N
    P -.-> S
    P -.-> R
    P -.-> Q
end
```

### Prerequisites

Make sure you have the following dependencies installed on your system:

- [CMake 4.0.0](https://cmake.org/)
- [C++23](https://en.cppreference.com/w/cpp/23)
- [Vulkan SDK](https://www.vulkan.org/)

## External Libraries

- [**GLFW**](https://github.com/glfw/glfw): For creating windows, receiving input, and managing OpenGL and Vulkan contexts.
- [**Google Test**](https://github.com/google/googletest): A testing framework for C++.
- [**ImGui**](https://github.com/ocornut/imgui): Immediate Mode Graphical User Interface for real-time debugging and tool development.
- [**stb**](https://github.com/nothings/stb): A set of single-file public domain libraries for graphics, image loading, and more.

## Contributing

Want to contribute? See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/bobis33/Cross-API-Engine/blob/main/LICENSE.md) file for details.
