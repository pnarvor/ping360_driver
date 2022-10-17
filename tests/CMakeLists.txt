
find_package(rtac_asio REQUIRED)

list(APPEND ping360_tests
    src/hello01.cpp
    src/hello02.cpp
    src/message_receive01.cpp
    src/serial_test01.cpp
)
foreach(filename ${ping360_tests})
    get_filename_component(name ${filename} NAME_WE)
    set(target_name ${name}_${PROJECT_NAME})
    add_executable(${target_name} ${filename})
    target_Link_libraries(${target_name} PRIVATE ping360_driver rtac_asio)
endforeach()



