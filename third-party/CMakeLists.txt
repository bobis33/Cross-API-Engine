add_library(cae-third-party INTERFACE)

#==================================== GoogleTest =====================================#
if (BUILD_CAE_TESTS)
    add_subdirectory(googletest)
endif()

#======================================= Utils =======================================#
add_subdirectory(utils)