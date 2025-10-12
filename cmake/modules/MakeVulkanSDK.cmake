include(FetchContent)
find_package(Vulkan 1.4)
if (Vulkan_FOUND)
    set(Vulkan_HEADERS Vulkan::Headers)
    set(Vulkan_LIBS Vulkan::Vulkan)
    set(Vulkan_BIN_DIR "")
    return()
endif ()
set(VULKAN_VERSION "1.4.329")

FetchContent_Declare(
        VulkanHeaders
        GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
        GIT_TAG v1.4.329
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(VulkanHeaders)

FetchContent_Declare(
        VulkanLoader
        GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Loader.git
        GIT_TAG v1.4.329
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(VulkanLoader)
