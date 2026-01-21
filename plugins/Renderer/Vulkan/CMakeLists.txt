project(cae-renderer-vulkan
        DESCRIPTION "CAE Vulkan Renderer Plugin"
        LANGUAGES C CXX
)

find_package(Vulkan QUIET CONFIG)
if(NOT Vulkan_FOUND)
    set(VULKAN_VERSION "v1.4.329")
    set(VULKAN_HEADERS_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
            VulkanHeaders
            GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Headers.git"
            GIT_TAG ${VULKAN_VERSION}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(VulkanHeaders)

    FetchContent_Declare(
            VulkanLoader
            GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Loader.git"
            GIT_TAG ${VULKAN_VERSION}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(VulkanLoader)

    set(Vulkan_INCLUDE_DIRS "${vulkanheaders_SOURCE_DIR}/include")
    set(Vulkan_LIBRARIES vulkan)
else()
    message(STATUS "Vulkan SDK found: ${Vulkan_INCLUDE_DIRS}")
endif()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

if(CAE_DYNAMIC_PLUGINS)
    add_library(${PROJECT_NAME} MODULE ${SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

if (CAE_ENABLE_LTO)
    cae_enable_lto(${PROJECT_NAME})
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
        cae-interfaces
        cae-utils
        ${Vulkan_LIBRARIES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)