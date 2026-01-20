project(cae-window-win32
        DESCRIPTION "CAE Win32 Window Plugin"
        LANGUAGES C CXX
)

if (NOT WIN32)
    message(WARNING "${PROJECT_NAME} can only be build on windows")
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

add_library(${PROJECT_NAME} SHARED ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        ${USER32_LIB}
        ${GDI32_LIB}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)