add_library(cae-modules INTERFACE)

add_subdirectory(Utils)
add_subdirectory(Engine)

target_link_libraries(cae-modules INTERFACE
        cae-utils
        cae-engine
)
target_include_directories(cae-modules SYSTEM INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/include"
)
