project(cae-window-cocoa
        DESCRIPTION "CAE Cocoa Window Plugin"
        LANGUAGES C CXX
)

if (NOT APPLE)
    message(WARNING "${PROJECT_NAME} can only be build on MacOS")
    return()
else ()
    enable_language(OBJCXX)
endif ()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.mm" "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        "-framework Cocoa"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)
