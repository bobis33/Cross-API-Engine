[![CI](https://github.com/bobis33/Cross-API-Engine/actions/workflows/ci.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/ci.yml)
[![Gitleaks](https://github.com/bobis33/Cross-API-Engine/actions/workflows/gitleaks.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/gitleaks.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/bobis33/Cross-API-Engine/blob/main/LICENSE.md)

# Cross-API-Engine | Rendering Engine with multiple dynamic backends

Cross-API-Engine is a rendering engine designed to support multiple graphics APIs dynamically.
It allows developers to switch between different rendering backends such as OpenGL, Vulkan, DirectX at runtime.
It is useful to do benchmarks during development or to support multiple platforms with different graphics APIs.

### Prerequisites

Make sure you have the following dependencies installed on your system:

- [CMake 3.27](https://cmake.org/)
- [C++20](https://en.cppreference.com/w/cpp/20)
- [Vulkan SDK](https://www.vulkan.org/)

## External Libraries

- [**GLFW**](https://github.com/glfw/glfw): For creating windows, receiving input, and managing OpenGL and Vulkan contexts.
- [**Google Test**](https://github.com/google/googletest): A testing framework for C++.
- [**ImGui**](https://github.com/ocornut/imgui): Immediate Mode Graphical User Interface for real-time debugging and tool development.
- [**stb**](https://github.com/nothings/stb): A set of single-file public domain libraries for graphics, image loading, and more.

## Commit Norms

| Commit Type | Description                                                                                                               |
|:------------|:--------------------------------------------------------------------------------------------------------------------------|
| build       | Changes that affect the build system or external dependencies (npm, make, etc.)                                           |
| ci          | Changes related to integration files and scripts or configuration (Travis, Ansible, BrowserStack, etc.)                   |
| feat        | Addition of a new feature                                                                                                 |
| fix         | Bug fix                                                                                                                   |
| perf        | Performance improvements                                                                                                  |
| refactor    | Modification that neither adds a new feature nor improves performance                                                     |
| style       | Change that does not affect functionality or semantics (indentation, formatting, adding space, renaming a variable, etc.) |
| docs        | Writing or updating documentation                                                                                         |
| test        | Addition or modification of tests                                                                                         |

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/bobis33/Cross-API-Engine/blob/main/LICENSE.md) file for details.