option(USE_CLANG_FORMAT "Enable clang-format" OFF)

if (NOT USE_CLANG_FORMAT)
    return()
endif()

if(TARGET ${PROJECT_NAME}-clang-format OR TARGET ${PROJECT_NAME}-clang-format-check)
    return()
endif()

find_program(CLANG_FORMAT_EXE NAMES clang-format REQUIRED)

add_custom_target(${PROJECT_NAME}-clang-format
        COMMAND ${CLANG_FORMAT_EXE} -style=file -i ${FILES_TO_FORMAT}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formatting sources with clang-format"
)
add_custom_target(${PROJECT_NAME}-clang-format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror -style=file ${FILES_TO_FORMAT}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking source formatting"
)