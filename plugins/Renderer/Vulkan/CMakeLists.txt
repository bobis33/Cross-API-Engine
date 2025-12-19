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

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

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
