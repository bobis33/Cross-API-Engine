project(cae-shader-frontend-hlsl
        DESCRIPTION "CAE HLSL Shader Frontend Plugin"
        LANGUAGES C CXX
)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} MODULE ${SOURCES})

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
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)