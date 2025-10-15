project(cae-x11
        DESCRIPTION "CAE X11 Window Plugin"
        LANGUAGES C CXX
)

find_package(X11 REQUIRED)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCE "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

add_library(${PROJECT_NAME} SHARED
        ${SOURCE}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${X11_INCLUDE_DIR}
)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE cae-modules ${X11_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)