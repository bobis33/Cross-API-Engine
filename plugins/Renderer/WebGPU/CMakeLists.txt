project(cae-renderer-webgpu
        DESCRIPTION "CAE WebGPU Renderer Plugin"
        LANGUAGES C CXX
)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)
