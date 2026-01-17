<img src="https://raw.githubusercontent.com/bobis33/Cross-API-Engine/main/assets/icons/icon-192x192.png" width="50" height="50" align="left" alt="Area logo"> 

# Cross-API-Engine 

[![CI](https://github.com/bobis33/Cross-API-Engine/actions/workflows/ci.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/ci.yml)
[![Gitleaks](https://github.com/bobis33/Cross-API-Engine/actions/workflows/gitleaks.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/gitleaks.yml)
[![CodeQL](https://github.com/bobis33/Cross-API-Engine/actions/workflows/codeql.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/codeql.yml)
[![Doxygen deployment](https://github.com/bobis33/Cross-API-Engine/actions/workflows/deploy-doxygen.yml/badge.svg)](https://github.com/bobis33/Cross-API-Engine/actions/workflows/deploy-doxygen.yml)
[![License](https://img.shields.io/github/license/bobis33/Cross-API-Engine.svg)](https://github.com/bobis33/Cross-API-Engine/blob/main/LICENSE.md)

---

**Cross-API-Engine** is a **fully modular** and **plugin-oriented** engine architecture designed to decouple every subsystem into independently loadable runtime modules.
Rather than focusing only on graphics API abstraction, the objective is to create an engine where every component can be swapped, extended, or replaced without modifying core code.
This enables rapid experimentation, platform portability, performance benchmarking, and true freedom in engine architecture research.

![Architecture Diagram](https://raw.githubusercontent.com/bobis33/Cross-API-Engine/main/assets/diagram.png)
## Supported Platforms
| Platform | Compiler                         | Build system    | Status |
|----------|----------------------------------|-----------------|--------|
| Windows  | MSVC (Visual Studio 2026 / 18.x) | MSBuild / Ninja | ✅      |
| Linux    | GCC ≥ 13                         | Ninja / Make    | ✅      |
| MacOS    | GCC ≥ 13 / Clang                 | Ninja / Make    | ✅      |
| iOS      |                                  |                 | ❌      |
| tvOS     |                                  |                 | ❌      |
| Android  |                                  |                 | ❌      |
| Web      |                                  |                 | ❌      |

## Build & Run

### Prerequisites

Make sure you have the following dependencies installed on your system:

- [CMake ≥ 4.0.0](https://cmake.org/)
- [C++23](https://en.cppreference.com/w/cpp/23)

### UNIX (Linux, MacOS)

```bash
./scripts/unix/build.sh release
```

```bash
./cmake-build-release/bin/cae -h          
Usage: cae [options]

Options:
  -h, --help              Show this help message
  -v, --version           Show version information
  -c, --config <path>     Specify JSON configuration file
```

### Windows

```powershell
cmake -S . -G "Visual Studio 18 2026" -A "x64" -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --config Release
```

```bash
cmake-build-release\bin\cae.exe -h          
Usage: cae.exe [options]

Options:
  -h, --help              Show this help message
  -v, --version           Show version information
  -c, --config <path>     Specify JSON configuration file
```

## External Libraries

- [Doxygen Awesome CSS](https://github.com/jothepro/doxygen-awesome-css): A custom CSS theme for Doxygen documentation.
- [Google Test](https://github.com/google/googletest): A testing framework for C++.
- [nlohmann-json](https://github.com/nlohmann/json): A JSON library for C++.

## Documentation

- [Modules](https://github.com/bobis33/Cross-API-Engine/blob/main/modules/README.md): Overview of available modules and their functionalities.
- [Plugins](https://github.com/bobis33/Cross-API-Engine/blob/main/plugins/README.md): Information on available plugins and how to create new ones.

Visit the [Doxygen documentation](https://bobis33.github.io/Cross-API-Engine/) for detailed information on architecture, modules, and usage.

## Contributing

Want to contribute? See [Contributing guidelines](https://github.com/bobis33/Cross-API-Engine/blob/main/CONTRIBUTING.md).

## Security

Please review our [Security Policy](https://github.com/bobis33/https://github.com/bobis33/Cross-API-Engine/blob/main/SECURITY.md) for more information on reporting security vulnerabilities.

## License

This project is licensed under the MIT License - see the [License](https://github.com/bobis33/Cross-API-Engine/blob/main//LICENSE.md) file for details.
