cmake_minimum_required(VERSION 3.15)
project(TrueGearCppSample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_path(WEBSOCKETPP_INCLUDE_DIR websocketpp/client.hpp)
if(NOT WEBSOCKETPP_INCLUDE_DIR)
    message(FATAL_ERROR "websocketpp headers not found. Install libwebsocketpp-dev or add the include path via -DWEBSOCKETPP_INCLUDE_DIR=<path>.")
endif()

add_executable(truegear_cpp_sample main.cpp)

target_include_directories(truegear_cpp_sample PRIVATE ${WEBSOCKETPP_INCLUDE_DIR})
target_link_libraries(truegear_cpp_sample PRIVATE Boost::system)

# websocketpp uses Asio via Boost, so ensure standalone Asio is not pulled in without Boost
if(TARGET websocketpp)
    target_link_libraries(truegear_cpp_sample PRIVATE websocketpp)
endif()
