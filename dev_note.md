
- https://docs.godotengine.org/en/4.3/contributing/development/core_and_modules/internal_rendering_architecture.html
- https://docs.godotengine.org/en/4.3/classes/class_renderingdevice.html
- https://docs.godotengine.org/fr/4.x/classes/class_renderingdevice.html

- https://docs.nvidia.com/

- https://github.com/DiligentGraphics
- https://www.youtube.com/watch?v=G9Gfrbf6wLI


## VK tools
 - Validation-layers
 - debug_utils
 - vkConfig
 - Shader assisted debuging
 - Syncval

## Windows build

**init**: `cmake.exe -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=C:/Users/masin/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -G Ninja -S C:\Users\masin\Projects\Cross-API-Engine -B C:\Users\masin\Projects\Cross-API-Engine\cmake-build-release`

**build**: `cmake.exe --build C:\Users\masin\Projects\Cross-API-Engine\cmake-build-release --target all -j 20
`