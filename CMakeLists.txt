cmake_minimum_required(VERSION 4.0.0)
project(cae
        VERSION 0.0.0
        DESCRIPTION "Cross-API graphics engine"
        LANGUAGES C CXX
)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/third-party/utils/cmake/modules")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/lib")
set(PLUGIN_DIR "${LIBRARY_OUTPUT_PATH}/plugins")
set(PATH_VERSION "${PROJECT_SOURCE_DIR}/include/CAE/Generated/Version.hpp")
set(TEMPLATE_PATH "${PROJECT_SOURCE_DIR}/third-party/utils/cmake/config/Version.hpp.in")

add_compile_definitions(PLUGINS_DIR="${PLUGIN_DIR}")

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE PROJECT_FILES
        ${SOURCES}
        ${HEADERS}
        "${PROJECT_SOURCE_DIR}/modules/*.cpp"
        "${PROJECT_SOURCE_DIR}/modules/*.hpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.cpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.hpp"
)
set(FILES_TO_FORMAT ${PROJECT_FILES})

include(MakeVersion)
include(MakeDoc)
include(ClangTidy)
include(ClangFormat)

add_subdirectory(third-party)
add_subdirectory(modules)
add_subdirectory(plugins)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} cae-third-party cae-modules)
target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/third-party/nlohmann-json/single_include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE cae-modules)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
if (NOT WIN32 AND NOT APPLE)
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