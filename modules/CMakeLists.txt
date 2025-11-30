add_library(cae-modules INTERFACE)

add_subdirectory(utils)

target_link_libraries(cae-modules INTERFACE
        utils
)
target_include_directories(cae-modules SYSTEM INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/Interfaces/include"
)
