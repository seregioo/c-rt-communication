cmake_minimum_required(VERSION 3.23)

project(c_rt_comunication
    VERSION 1.0
    DESCRIPTION "RealTime communication c project"
    LANGUAGES C CXX)

#####################
#   Dependencies    #
#####################


##################
#   RT Result    #
##################

target_sources(c_rt_comunication
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

add_library(rt_result STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rt_result/result.c
)

target_include_directories(rt_result
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#############
# RT Thread #
#############

add_library(rt_thread
            STATIC src/rt_thread/thread.c)

set(RT_BACKEND "POSIX" CACHE STRING "RT backend")
set_property(CACHE RT_BACKEND PROPERTY STRINGS POSIX PREEMPT_RT XENOMAI)

if(RT_BACKEND STREQUAL "POSIX")
    target_sources(rt_thread PRIVATE
        src/rt_thread/core/posix.c)
    target_compile_definitions(rt_thread PRIVATE POSIX_BACKEND)
elseif(RT_BACKEND STREQUAL "PREEMPT_RT")
    target_sources(rt_thread PRIVATE
        src/rt_thread/core/preempt_rt.c)
    target_compile_definitions(rt_thread PRIVATE PREEMPT_RT_BACKEND)
elseif(RT_BACKEND STREQUAL "XENOMAI")
    target_sources(rt_thread PRIVATE
        src/rt_thread/core/xenomai.c)
    target_compile_definitions(rt_thread PRIVATE XENOMAI_BACKEND)
else()
    message(FATAL_ERROR "Invalid RT_BACKEND")
endif()

target_include_directories(rt_thread PUBLIC include/)

#############
#   Main    #
#############

add_executable(c_rt_comunication
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

target_link_libraries(c_rt_comunication
    PRIVATE
        rt_result
)

################
#   RT Tests    #
################

enable_testing()
add_subdirectory(test)
