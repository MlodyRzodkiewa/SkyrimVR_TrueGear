# Minimal spdlog config for vendored headers
set(_spdlog_root "${CMAKE_CURRENT_LIST_DIR}/..")

if(NOT TARGET spdlog::spdlog)
    add_library(spdlog INTERFACE)
    add_library(spdlog::spdlog ALIAS spdlog)
    target_include_directories(spdlog INTERFACE "${_spdlog_root}/extern")
    target_link_libraries(spdlog INTERFACE fmt::fmt)
endif()
