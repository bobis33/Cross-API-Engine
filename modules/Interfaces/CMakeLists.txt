add_library(cae-interfaces INTERFACE)

target_include_directories(cae-interfaces SYSTEM INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
