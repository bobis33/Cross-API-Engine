project(cae-shader-frontend-glsl
        DESCRIPTION "CAE GLSL Shader Frontend Plugin"
        LANGUAGES C CXX
)

find_package(glslang QUIET CONFIG)
if (NOT glslang_FOUND)
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

add_library(${PROJECT_NAME} MODULE ${SOURCES})

if (CAE_ENABLE_LTO)
    cae_enable_lto(${PROJECT_NAME})
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
        cae-interfaces
        cae-utils
        glslang::glslang
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)