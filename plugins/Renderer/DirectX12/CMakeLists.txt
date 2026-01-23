project(cae-renderer-directx12
        DESCRIPTION "CAE DirectX12 Renderer Plugin"
        LANGUAGES C CXX
)

if (NOT WIN32)
    message(WARNING "${PROJECT_NAME} can only be build on Windows")
    return()
endif ()

FetchContent_Declare(
        directx_headers
        GIT_REPOSITORY "https://github.com/microsoft/DirectX-Headers.git"
        GIT_TAG "v1.618.2"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(DXHEADERS_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(DXHEADERS_BUILD_GOOGLE_TEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(directx_headers)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} MODULE ${SOURCES})

if (CAE_ENABLE_LTO)
    cae_enable_lto(${PROJECT_NAME})
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${glm_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
        cae-interfaces
        cae-utils
        d3d12
        dxgi
        glm::glm
        Microsoft::DirectX-Headers
        Microsoft::DirectX-Guids
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)
