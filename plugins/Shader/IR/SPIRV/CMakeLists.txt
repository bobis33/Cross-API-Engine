## Need spirv-cross for cross-compilation,
## maybe spirv-val for validation and spirv-opt for optimization

project(cae-spirv
        DESCRIPTION "CAE SPIRV Shader Intermediate Representation Plugin"
        LANGUAGES C CXX
)

include(FetchContent)

FetchContent_Declare(
        spirv-headers
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
        GIT_TAG main
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(spirv-headers)

## set(SPIRV_SKIP_TESTS ON CACHE BOOL "")
## set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "")
## FetchContent_Declare(
##         spirv-tools
##         GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
##         GIT_TAG vulkan-sdk-1.4.335.0
## )
## set(SPIRV_TOOLS_INCLUDE_DIR ${spirv-headers_SOURCE_DIR})
## FetchContent_MakeAvailable(spirv-tools)

FetchContent_Declare(
        spirv-cross
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
        GIT_TAG vulkan-sdk-1.4.335.0
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(spirv-cross)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ## "${spirv-tools_SOURCE_DIR}/include"
        "${spirv-cross_SOURCE_DIR}/include"
)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        ## SPIRV-Tools-opt
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl
        spirv-cross-msl
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)