include_directories("../")

file(GLOB test_src "*.cpp")

foreach(file ${test_src})
    get_filename_component(TEST_NAME ${file} NAME_WE)
    message(STATUS "add ${TEST_NAME} from ${file}")
    add_executable(${TEST_NAME} ${file})
    target_link_libraries(${TEST_NAME} pthread)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 30) 
endforeach()

