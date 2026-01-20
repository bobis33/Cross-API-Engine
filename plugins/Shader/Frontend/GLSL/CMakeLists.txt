project(cae-shaders-frontend-glsl
        DESCRIPTION "CAE GLSL Shader Frontend Plugin"
        LANGUAGES C CXX
)

find_package(glslang QUIET CONFIG)
if (NOT glslang_FOUND)
    include(FetchContent)
    set(BUILD_EXTERNAL OFF CACHE BOOL "" FORCE)
    set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)
    set(ENABLE_OPT OFF CACHE BOOL "" FORCE)
    set(ENABLE_SPIRV ON CACHE BOOL "" FORCE)
    set(ENABLE_GLSLANG_BINARIES ON CACHE BOOL "" FORCE)
    set(GLSLANG_TESTS OFF CACHE BOOL "" FORCE)
    set(ENABLE_PCH ON CACHE BOOL "" FORCE)
    FetchContent_Declare(
            glslang
            GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
            GIT_TAG 16.1.0
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
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        glslang::glslang
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)