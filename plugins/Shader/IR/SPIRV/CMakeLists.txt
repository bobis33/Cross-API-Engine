## Need spirv-val for validation and spirv-opt for optimization

## set(SPIRV_SKIP_TESTS ON CACHE BOOL "")
## set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "")
## FetchContent_Declare(
##         spirv-tools
##         GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
##         GIT_TAG vulkan-sdk-1.4.335.0
## )
## set(SPIRV_TOOLS_INCLUDE_DIR ${spirv-headers_SOURCE_DIR})
## FetchContent_MakeAvailable(spirv-tools)

project(cae-shaders-ir-spirv
        DESCRIPTION "CAE SPIRV Shader Intermediate Representation Plugin"
        LANGUAGES C CXX
)

find_package(SPIRV-Headers QUIET CONFIG)

if(NOT SPIRV-Headers_FOUND)
    set(SPIRV_HEADERS_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
            spirv-headers
            GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Headers.git"
            GIT_TAG "vulkan-sdk-1.4.335.0"
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(spirv-headers)
    set(SPIRV_HEADERS_INCLUDE_DIR "${spirv-headers_SOURCE_DIR}/include")
else()
    message(STATUS "Using installed SPIRV-Headers")
    set(SPIRV_HEADERS_INCLUDE_DIR "${SPIRV-Headers_INCLUDE_DIRS}")
endif()

find_package(SPIRV-Cross QUIET CONFIG)

if(NOT SPIRV-Cross_FOUND)
    set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
    set(SPIRV_CROSS_CLI OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
            spirv-cross
            GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Cross.git"
            GIT_TAG "vulkan-sdk-1.4.335.0"
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(spirv-cross)

    set(SPIRV_CROSS_INCLUDE_DIR "${spirv-cross_SOURCE_DIR}/include")
    set(SPIRV_CROSS_LIBS
            spirv-cross-core
            spirv-cross-glsl
            spirv-cross-hlsl
            spirv-cross-msl
    )
else()
    message(STATUS "Using installed SPIRV-Cross")
    set(SPIRV_CROSS_INCLUDE_DIR "${SPIRV-Cross_INCLUDE_DIRS}")
    set(SPIRV_CROSS_LIBS
            spirv-cross-core
            spirv-cross-glsl
            spirv-cross-hlsl
            spirv-cross-msl
            ## SPIRV-Tools-opt
            ## SPIRV-Tools-val
    )
endif()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} MODULE ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${SPIRV_HEADERS_INCLUDE_DIR}
        ${SPIRV_CROSS_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
        cae-interfaces
        cae-utils
        ${SPIRV_CROSS_LIBS}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)
