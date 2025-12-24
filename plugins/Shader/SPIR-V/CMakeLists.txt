project(cae-spriv
        DESCRIPTION "CAE SPIR-V Shader Plugin"
        LANGUAGES C CXX
)

find_package(glslang QUIET)
find_package(SPIRV-Tools QUIET)

if (NOT SPIRV-Tools_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            spirv-headers
            GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Headers.git"
            GIT_TAG main
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(spirv-headers)
    FetchContent_Declare(
            spirv-tools
            GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Tools.git"
            GIT_TAG main
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(spirv-tools)
endif ()

if (NOT glslang_FOUND)
    include(FetchContent)

    set(ENABLE_OPT ON CACHE BOOL "" FORCE)
    set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
            glslang
            GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
            GIT_TAG main
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(glslang)
endif ()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        glslang
        SPIRV
        SPIRV-Tools
        SPIRV-Tools-opt
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)