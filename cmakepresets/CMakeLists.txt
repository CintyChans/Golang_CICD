if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "The vcpkg triplet")
endif()

cmake_minimum_required(VERSION 3.0)
project(test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS"${CMAKE_CXX_FLAGS} /std:c++17")
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
add_executable(main)
target_sources(main PRIVATE test.cpp)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(main PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE Eigen3::Eigen)

find_package(argparse CONFIG REQUIRED)
target_link_libraries(main PRIVATE argparse::argparse)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(main PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(main PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

find_package(asio CONFIG REQUIRED)
target_link_libraries(main PRIVATE asio asio::asio)

find_package(eclipse-paho-mqtt-c CONFIG REQUIRED)
target_link_libraries(resr PRIVATE eclipse-paho-mqtt-c::paho-mqtt3a eclipse-paho-mqtt-c::paho-mqtt3c eclipse-paho-mqtt-c::paho-mqtt3as eclipse-paho-mqtt-c::paho-mqtt3cs)


set_property(TARGET main PROPERTY CXX_STANDARD 20)
