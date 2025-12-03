cmake_minimum_required(VERSION 3.20)

project(TrueGearPluginVR LANGUAGES CXX)

set(CMAKE_SKIP_INSTALL_RULES ON)

# =====================================================================
# C++ CONFIG
# =====================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required by fmt/spdlog + UTF-8 handling on MSVC
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Make sure our vendored package configs are discoverable
list(PREPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# =====================================================================
# VENDORED PACKAGES
# =====================================================================
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# =====================================================================
# TrueGearPluginVR.dll
# =====================================================================
add_library(TrueGearPluginVR SHARED
    src/TrueGearPlugin.cpp
    src/TrueGearWebsocket.cpp
    src/EffectDatabase.cpp
)

target_compile_definitions(TrueGearPluginVR PRIVATE
    SKYRIMVR
    _SKYRIMVR
    SKSE_SUPPORT_XBYAK
)

target_include_directories(TrueGearPluginVR PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/extern/nlohmann"
)

target_link_libraries(TrueGearPluginVR PRIVATE
    spdlog::spdlog
    fmt::fmt
)

# =====================================================================
# OUTPUT SETTINGS
# =====================================================================
set_target_properties(TrueGearPluginVR PROPERTIES
    OUTPUT_NAME "TrueGearPluginVR"
    PREFIX ""
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
