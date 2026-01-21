add_library(cae-compile-options INTERFACE)

target_compile_features(cae-compile-options INTERFACE cxx_std_23)

option(CAE_STRICT_WARNINGS "Enable strict warning level" OFF)
option(CAE_ENABLE_SANITIZERS "Enable address and undefined sanitizers" OFF)
option(CAE_ENABLE_LTO "Enable LTO on final targets" OFF)

target_compile_options(cae-compile-options INTERFACE
        # Strict warnings
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<BOOL:${CAE_STRICT_WARNINGS}>,$<NOT:$<PLATFORM_ID:Android,Emscripten,iOS>>>:
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wold-style-cast
        -Woverloaded-virtual
        -Wnull-dereference
        -Wformat=2
        -Wundef
        -Wswitch-default
        >
        # mobile / web
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<BOOL:${CAE_STRICT_WARNINGS}>,$<PLATFORM_ID:Android,Emscripten,iOS>>:
        -Wall
        -Wextra
        -Wshadow
        -Wsign-conversion
        -Wold-style-cast
        >
        # Default warnings
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<NOT:$<BOOL:${CAE_STRICT_WARNINGS}>>>:
        -Wall
        -Wextra
        >
        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /permissive-
        /Zc:__cplusplus
        /Zc:preprocessor
        >
)

# GCC / Clang
target_compile_options(cae-compile-options INTERFACE
        $<$<AND:$<CONFIG:RelWithDebInfo>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-O2>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-O0 -g>
)

# MSVC
target_compile_options(cae-compile-options INTERFACE
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2>
        $<$<AND:$<CONFIG:RelWithDebInfo>,$<CXX_COMPILER_ID:MSVC>>:/O2 /Zi>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od /Zi>
)

target_compile_definitions(cae-compile-options INTERFACE
        $<$<CONFIG:Debug>:
        CAE_DEBUG
        >
)

if(CAE_ENABLE_SANITIZERS)
    target_compile_options(cae-compile-options INTERFACE
            $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<NOT:$<PLATFORM_ID:Android,Emscripten,Windows>>>:
            -fsanitize=address,undefined
            >
    )
    target_link_options(cae-compile-options INTERFACE
            $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<NOT:$<PLATFORM_ID:Android,Emscripten,Windows>>>:
            -fsanitize=address,undefined
            >
    )
endif()

function(cae_enable_lto target)
    if (CAE_ENABLE_LTO)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT CAE_LTO_SUPPORTED OUTPUT CAE_LTO_ERROR)
        if (CAE_LTO_SUPPORTED)
            set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        else()
            message(WARNING "CAE: LTO not supported: ${CAE_LTO_ERROR}")
        endif()
    endif()
endfunction()

if (EMSCRIPTEN)
    target_compile_definitions(cae-compile-options INTERFACE
            CAE_PLATFORM_WEB
            CAE_PLATFORM_EMSCRIPTEN
    )
elseif (ANDROID)
    target_compile_definitions(cae-compile-options INTERFACE CAE_PLATFORM_ANDROID)
elseif (APPLE)
    if (IOS)
        target_compile_definitions(cae-compile-options INTERFACE CAE_PLATFORM_IOS)
    else()
        target_compile_definitions(cae-compile-options INTERFACE CAE_PLATFORM_MACOS)
    endif()
elseif (WIN32)
    target_compile_definitions(cae-compile-options INTERFACE
            CAE_PLATFORM_WINDOWS
            NOMINMAX
            WIN32_LEAN_AND_MEAN
    )
elseif (UNIX)
    target_compile_definitions(cae-compile-options INTERFACE CAE_PLATFORM_LINUX)
endif()

