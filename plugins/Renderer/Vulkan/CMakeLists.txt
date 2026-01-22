project(cae-renderer-vulkan
        DESCRIPTION "CAE Vulkan Renderer Plugin"
        LANGUAGES C CXX
)

find_package(Vulkan QUIET CONFIG)
if(NOT Vulkan_FOUND)
    include(FetchContent)
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

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        ${Vulkan_LIBRARIES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)
