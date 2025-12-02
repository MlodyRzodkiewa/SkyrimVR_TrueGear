cmake_minimum_required(VERSION 3.19)

option(REX_OPTION_INI "Enables ini config support for REX." OFF)
option(REX_OPTION_JSON "Enables json config support for REX." OFF)
option(REX_OPTION_TOML "Enables toml config support for REX." OFF)
option(SKSE_SUPPORT_XBYAK "Enables trampoline support for Xbyak." OFF)
option(BUILD_SKYRIMVR "Build for Skyrim VR" OFF)
option(SKYRIM_SUPPORT_AE "Enables support for Skyrim AE" OFF)

if (NOT BUILD_SKYRIMVR)
    project(CommonLibSSE LANGUAGES CXX)
else()
    project(CommonLibVR LANGUAGES CXX)
    add_compile_definitions(SKYRIMVR)
endif()

include(GNUInstallDirs)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "in-source builds are not allowed")
endif()

#
# VCPKG packages
#
# ❗ USUWAMY find_package(binary_io) – binary_io jest lokalne
find_package(fmt REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)

#
# Lokalny binary_io
#
#add_subdirectory(${CMAKE_SOURCE_DIR}/extern/binary_io)

#
# Source list
#
include(cmake/sourcelist.cmake)

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES ${SOURCES}
)

add_library(
    "${PROJECT_NAME}"
    STATIC
    ${SOURCES}
    .clang-format
    CommonLibSSE.natvis
)

add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_compile_definitions(
    "${PROJECT_NAME}"
    PUBLIC
        WINVER=0x0601
        _WIN32_WINNT=0x0601
        "$<$<BOOL:${REX_OPTION_INI}>:REX_OPTION_INI=1>"
        "$<$<BOOL:${REX_OPTION_JSON}>:REX_OPTION_JSON=1>"
        "$<$<BOOL:${REX_OPTION_TOML}>:REX_OPTION_TOML=1>"
        "$<$<BOOL:${SKSE_SUPPORT_XBYAK}>:SKSE_SUPPORT_XBYAK=1>"
        "$<$<BOOL:${SKYRIM_SUPPORT_AE}>:SKYRIM_SUPPORT_AE=1>"
)

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_23)

if (MSVC)
    target_compile_options(
        "${PROJECT_NAME}"
        PRIVATE
            /we4715
            /wd4005 /wd4061 /wd4200 /wd4201 /wd4265 /wd4266 /wd4371 /wd4514
            /wd4582 /wd4583 /wd4623 /wd4625 /wd4626 /wd4710 /wd4711 /wd4820
            /wd5026 /wd5027 /wd5045 /wd5053 /wd5204 /wd5220
    )
endif()

#
# Includes
#

if (NOT BUILD_SKYRIMVR)
    # SE
    target_include_directories(
        "${PROJECT_NAME}"
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )
else()
    # VR — OpenVR headers (lokalne)
    set(OPENVR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/openvr/headers)

    # BOOST headers (lokalne, header-only)
    set(BOOST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/boost)

    target_include_directories(
        "${PROJECT_NAME}"
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
            "$<BUILD_INTERFACE:${OPENVR_INCLUDE_DIR}>"
            "$<BUILD_INTERFACE:${BOOST_INCLUDE_DIR}>"
			"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/binary_io/include>"
    )
endif()

#
# Link libs
#
target_link_libraries(
    "${PROJECT_NAME}"
    PUBLIC
        #binary_io      # binary_io dodane w głównym CMakeLists
        spdlog::spdlog
        fmt::fmt
        Advapi32.lib
        bcrypt.lib
        D3D11.lib
        d3dcompiler.lib
        Dbghelp.lib
        DXGI.lib
        Ole32.lib
        Version.lib
)

target_precompile_headers(
    "${PROJECT_NAME}"
    PRIVATE
        include/SKSE/Impl/PCH.h
)

install(
    TARGETS "${PROJECT_NAME}"
    EXPORT "${PROJECT_NAME}-targets"
)

install(
    EXPORT "${PROJECT_NAME}-targets"
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

configure_file(
    cmake/config.cmake.in
    "${PROJECT_NAME}Config.cmake"
    @ONLY
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
    DIRECTORY
        "include/RE"
        "include/REL"
        "include/REX"
        "include/SKSE"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
