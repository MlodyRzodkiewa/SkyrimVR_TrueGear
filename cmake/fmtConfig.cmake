# Minimal fmt config for vendored headers
set(_fmt_root "${CMAKE_CURRENT_LIST_DIR}/..")

if(NOT TARGET fmt::fmt)
    add_library(fmt INTERFACE)
    add_library(fmt::fmt ALIAS fmt)
    target_include_directories(fmt INTERFACE "${_fmt_root}/extern")
endif()
