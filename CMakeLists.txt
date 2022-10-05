cmake_minimum_required(VERSION 3.6)
project(ping360_driver VERSION 0.1)

if(${CMAKE_VERSION} VERSION_LESS 3.8.2)
    message(WARNING "You are using a very old CMake version. This will work but please upgrade.")
    set(CMAKE_CXX_STANDARD 14)
endif()

find_package(Boost COMPONENTS system thread REQUIRED)
set(Boost_VERSION_STRING "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
message(STATUS "Boost location : ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost version  : ${Boost_VERSION_STRING}")

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
if(${Boost_VERSION_STRING} VERSION_LESS 1.66.0)
    target_compile_definitions(ping360_driver PRIVATE BOOST_OLD_VERSION)
endif()
if(${CMAKE_VERSION} VERSION_GREATER 3.8.2)
    target_compile_features(ping360_driver PUBLIC cxx_std_14)
endif()

add_subdirectory(tests)
