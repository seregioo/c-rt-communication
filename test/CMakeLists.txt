include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(rt_result_test
    rt_result_test.cpp
)

target_link_libraries(rt_result_test
    PRIVATE
        rt_result
        GTest::gtest_main
)

###########
# RT SPSC #
###########

add_executable(rt_spsc_test
    rt_spsc_test.cpp
)

target_link_libraries(rt_spsc_test
    PRIVATE
        rt_spsc
        GTest::gtest_main
)

#############
# RT Plugin #
#############

add_executable(rt_plugin_test
    rt_plugin/rt_plugin_test.cpp
    rt_plugin/rt_plugin_abi_test.cpp
)

target_link_libraries(rt_plugin_test
    PRIVATE
        rt_plugin 
        GTest::gtest_main
)


include(GoogleTest)

gtest_discover_tests(rt_result_test)
gtest_discover_tests(rt_spsc_test)
gtest_discover_tests(rt_plugin_test)
