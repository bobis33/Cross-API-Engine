cmake_minimum_required(VERSION 4.0.0)
project(cae
        VERSION 0.0.0
        DESCRIPTION "Cross-API graphics engine"
        LANGUAGES C CXX
)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/third-party/utils/cmake/modules")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/lib")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

set(PATH_VERSION "${CMAKE_SOURCE_DIR}/include/CAE/Generated/Version.hpp")
set(TEMPLATE_PATH "${CMAKE_SOURCE_DIR}/third-party/utils/cmake/config/Version.hpp.in")
file(GLOB_RECURSE FILES_TO_FORMAT
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/plugins/Renderer/OpenGL/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/plugins/Renderer/OpenGL/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/plugins/Renderer/Vulkan/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/plugins/Renderer/Vulkan/include/*.hpp"
        "${CMAKE_SOURCE_DIR}/plugins/Window/GLFW/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/plugins/Window/GLFW/include/*.hpp"
)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.hpp")

include(MakeVersion)
include(MakeDoc)
include(ClangTidy)
include(ClangFormat)

add_subdirectory(third-party)
add_subdirectory(modules)
add_subdirectory(plugins)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_dependencies(${PROJECT_NAME} cae-third-party cae-modules)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE cae-modules)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
if (NOT WIN32)
    set(WARNING_FLAGS
            -Wall
            -Wextra
            -Wdeprecated-copy
            -Wmisleading-indentation
            -Wnull-dereference
            -Woverloaded-virtual
            -Wpedantic
            -Wshadow
            -Wsign-conversion
            -Wnon-virtual-dtor
            -Wunused
            -Wcast-align
            -Wno-padded
            -Wconversion
            -Wformat
            -Winit-self
            -Wmissing-include-dirs
            -Wold-style-cast
            -Wredundant-decls
            -Wswitch-default
            -Wundef
    )
else ()
    if (MSVC)
        set(WARNING_FLAGS /W3)
    else()
        set(WARNING_FLAGS -Wno-error)
    endif()
endif()
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS} /O2)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS} -O3)
endif ()
add_compile_definitions(PLUGINS_DIR="${LIBRARY_OUTPUT_PATH}")
