project(cae-shader-frontend-hlsl
        DESCRIPTION "CAE HLSL Shader Frontend Plugin"
        LANGUAGES C CXX
)

include(FetchContent)

if (WIN32)
    set(DXC_URL
            "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2505.1/dxc_2025_07_14.zip"
    )
elseif(UNIX)
    set(DXC_URL
           "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2505.1/linux_dxc_2025_07_14.x86_64.tar.gz"
    )
else()
    message(FATAL_ERROR "DXC not supported on this platform")
endif()

FetchContent_Declare(
        dxcompiler_bin
        URL ${DXC_URL}
)

FetchContent_GetProperties(dxcompiler_bin)

if (NOT dxcompiler_bin_POPULATED)
    FetchContent_MakeAvailable(dxcompiler_bin)
endif()

set(DXC_ROOT ${dxcompiler_bin_SOURCE_DIR})

add_library(dxcompiler SHARED IMPORTED GLOBAL)

if (WIN32)
    set_target_properties(dxcompiler PROPERTIES
            IMPORTED_LOCATION "${DXC_ROOT}/bin/x64/dxcompiler.dll"
            IMPORTED_IMPLIB  "${DXC_ROOT}/lib/x64/dxcompiler.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${DXC_ROOT}/inc"
    )
else()
    set_target_properties(dxcompiler PROPERTIES
            IMPORTED_LOCATION "${DXC_ROOT}/bin/libdxcompiler.so"
            INTERFACE_INCLUDE_DIRECTORIES "${DXC_ROOT}/inc"
    )
endif()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} MODULE ${SOURCES})

if (CAE_ENABLE_LTO)
    cae_enable_lto(${PROJECT_NAME})
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${dxcompiler_SOURCE_DIR}/include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
        cae-interfaces
        cae-utils
        dxcompiler
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)