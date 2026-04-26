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

include(GoogleTest)

gtest_discover_tests(rt_result_test)
