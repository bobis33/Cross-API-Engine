project(cae-vulkan
        DESCRIPTION "CAE Vulkan Renderer Plugin"
        LANGUAGES C CXX
)

include(FetchContent)
set(VULKAN_VERSION "v1.4.329")

FetchContent_Declare(
        VulkanHeaders
        GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Headers.git"
        GIT_TAG ${VULKAN_VERSION}
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(VulkanHeaders)

FetchContent_Declare(
        VulkanLoader
        GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Loader.git"
        GIT_TAG ${VULKAN_VERSION}
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(VulkanLoader)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCE "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

add_library(${PROJECT_NAME} SHARED
        ${SOURCE}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${vulkanheaders_SOURCE_DIR}/include"
)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE cae-modules vulkan)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)
