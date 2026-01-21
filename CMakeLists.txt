cmake_minimum_required(VERSION 4.0.0)

project(cae
        VERSION 0.0.0
        DESCRIPTION "Cross-API graphics engine"
        HOMEPAGE_URL "https://bobis33.github.io/Cross-API-Engine/"
        LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

option(CAE_BUILD_DOC_HTML "Build html documentation" OFF)
option(CAE_BUILD_DOC_PDF "Build pdf documentation" OFF)
option(CAE_DYNAMIC_PLUGINS "Enable dynamic plugins" OFF)
option(CAE_CLANG_FORMAT "Enable clang-format" OFF)
option(CAE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)

file(GLOB_RECURSE PROJECT_FILES
        ${SOURCES}
        "${PROJECT_SOURCE_DIR}/apps/*.cpp"
        "${PROJECT_SOURCE_DIR}/apps/*.hpp"
        "${PROJECT_SOURCE_DIR}/modules/*.cpp"
        "${PROJECT_SOURCE_DIR}/modules/*.hpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.cpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.hpp"
)

include(FetchContent)

include(MakeVersion)
include(MakeDoc)
include(ClangTidy)
include(ClangFormat)
include(CopyAssets)
include(CompileOptions)

if (CAE_CLANG_FORMAT)
    format_project(${PROJECT_FILES})
endif ()

if (CAE_CLANG_TIDY)
    run_clang_tidy(${PROJECT_FILES})
endif ()

if(EMSCRIPTEN OR IOS)
    set(CAE_DYNAMIC_PLUGINS OFF CACHE BOOL "" FORCE)
endif()

if (CAE_BUILD_DOC_HTML OR CAE_BUILD_DOC_PDF)
    find_package(Doxygen REQUIRED)
    cae_download_extra_css()
    if (CAE_BUILD_DOC_HTML)
        cae_add_doxygen_html(cae-doc-html ${PROJECT_FILES})
    elseif (CAE_BUILD_DOC_PDF)
        cae_add_doxygen_pdf(cae-doc-pdf ${PROJECT_FILES})
    endif ()
endif ()

add_subdirectory(modules)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(apps)
