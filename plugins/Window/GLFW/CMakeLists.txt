project(cae-window-glfw
        DESCRIPTION "CAE GLFW Window Plugin"
        LANGUAGES C CXX
)

if (APPLE)
    enable_language(OBJC)
    enable_language(OBJCXX)
endif()

find_package(GLFW3 QUIET CONFIG)
if(NOT GLFW3_FOUND)
    message(STATUS "GLFW not found.")

    include(FetchContent)
    set(BUILD_SHARED_LIBS OFF)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_VULKAN_STATIC OFF)
    set(GLFW_BUILD_WAYLAND OFF)
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY "https://github.com/glfw/glfw.git"
            GIT_TAG 3.4
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(glfw)

    set(GLFW_INCLUDE_DIR "${glfw_SOURCE_DIR}/include")
    set(GLFW_LIB glfw)
else()
    message(STATUS "Using installed GLFW3")
    set(GLFW_INCLUDE_DIR "${GLFW3_INCLUDE_DIRS}")
    set(GLFW_LIB "GLFW::GLFW")
endif()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${GLFW_INCLUDE_DIR}
)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        ${GLFW_LIB}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)
