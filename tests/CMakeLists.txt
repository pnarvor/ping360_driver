

list(APPEND ping360_tests
    src/hello01.cpp
)
foreach(filename ${ping360_tests})
    get_filename_component(name ${filename} NAME_WE)
    set(target_name ${name}_${PROJECT_NAME})
    add_executable(${target_name} ${filename})
    target_Link_libraries(${target_name} PRIVATE ping360_driver)
endforeach()



