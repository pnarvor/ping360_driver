cmake_minimum_required(VERSION 3.16)
project(ping360_driver VERSION 0.1)

find_package(Boost COMPONENTS system thread REQUIRED)
message(STATUS "Boost version  : ${Boost_VERSION}")
message(STATUS "Boost location : ${Boost_INCLUDE_DIRS}")

add_library(ping360_driver SHARED
    src/AsyncService.cpp
    src/ClientUDP.cpp
    src/ClientSerial.cpp
    src/PingClient.cpp
)
target_include_directories(ping360_driver PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(ping360_driver INTERFACE
    Boost::system
    Boost::thread
)
target_compile_features(ping360_driver PUBLIC cxx_std_14)

add_subdirectory(tests)
