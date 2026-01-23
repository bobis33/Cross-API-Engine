project(cae-window-win32
        DESCRIPTION "CAE Win32 Window Plugin"
        LANGUAGES C CXX
)

if (NOT WIN32)
    message(WARNING "${PROJECT_NAME} can only be build on Windows")
    return()
endif ()

add_compile_definitions(UNICODE _UNICODE)

find_library(USER32_LIB user32)
find_library(GDI32_LIB gdi32)

if(NOT USER32_LIB)
    set(USER32_LIB "user32")
endif()
if(NOT GDI32_LIB)
    set(GDI32_LIB "gdi32")
endif()


file(GLOB_RECURSE SOURCE "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} MODULE ${SOURCE})

if (CAE_ENABLE_LTO)
    cae_enable_lto(${PROJECT_NAME})
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
        cae-interfaces
        cae-utils
        ${USER32_LIB}
        ${GDI32_LIB}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)