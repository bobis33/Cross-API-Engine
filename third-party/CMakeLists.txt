#==================================== GoogleTest =====================================#
add_library(cae-third-party INTERFACE)
if (BUILD_CAE_TESTS)
    add_subdirectory(googletest)
endif()
#======================================= ImGui =======================================#
## set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
## add_library(imgui STATIC
##         "${IMGUI_DIR}/imgui.cpp"
##         "${IMGUI_DIR}/imgui_draw.cpp"
##         "${IMGUI_DIR}/imgui_widgets.cpp"
##         "${IMGUI_DIR}/imgui_tables.cpp"
##         "${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp" ## for opengl ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
##         "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
## )
## target_include_directories(imgui PRIVATE
##         ${IMGUI_DIR}
##         "${CMAKE_CURRENT_SOURCE_DIR}/glfw/include"
## )
## target_link_libraries(imgui PRIVATE Vulkan::Vulkan)
## target_compile_features(imgui PRIVATE cxx_std_23)
#======================================= Utils =======================================#
add_subdirectory(utils)