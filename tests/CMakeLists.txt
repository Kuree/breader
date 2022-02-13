function(add_test target)
    add_executable(${target} ${target}.cc)
    target_link_libraries(${target} gtest breader gtest_main)
    gtest_discover_tests(${target})
    target_include_directories(${target} PRIVATE
            ../include
            ../extern/googletest/googletest/include)
endfunction()

add_test(test_io)