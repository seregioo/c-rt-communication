cmake_minimum_required(VERSION 3.23)

project(c_rt_comunication
    VERSION 1.0
    DESCRIPTION "RealTime communication c project"
    LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                 Debug Release RelWithDebInfo MinSizeRel)
endif()

#####################
#   Dependencies    #
#####################
include(FetchContent)
find_package(Threads REQUIRED)

##################
#   RT Result    #
##################


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
            STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/rt_thread/result.c
                   ${CMAKE_CURRENT_SOURCE_DIR}/src/rt_thread/thread.c)

set(RT_BACKEND "POSIX" CACHE STRING "RT backend")
set_property(CACHE RT_BACKEND PROPERTY STRINGS POSIX PREEMPT_RT XENOMAI)

if(RT_BACKEND STREQUAL "POSIX")
    target_sources(rt_thread PRIVATE
        src/rt_thread/core/posix.c)
    target_compile_definitions(rt_thread PRIVATE POSIX_BACKEND)
    target_link_libraries(rt_thread PUBLIC Threads::Threads)
elseif(RT_BACKEND STREQUAL "PREEMPT_RT")
    target_sources(rt_thread PRIVATE
        src/rt_thread/core/preempt_rt.c)
    target_compile_definitions(rt_thread PRIVATE PREEMPT_RT_BACKEND)
    target_link_libraries(rt_thread PUBLIC Threads::Threads)
    target_compile_options(rt_thread PRIVATE
        $<$<NOT:$<CONFIG:Debug>>:-O2>)
elseif(RT_BACKEND STREQUAL "XENOMAI")
    target_sources(rt_thread PRIVATE
        src/rt_thread/core/xenomai.c)
    target_compile_definitions(rt_thread PRIVATE XENOMAI_BACKEND)
else()
    message(FATAL_ERROR "Invalid RT_BACKEND")
endif()

target_include_directories(rt_thread PUBLIC include/)

##########
# RT DAQ #
##########

add_library(rt_daq
            STATIC src/rt_daq/result.c
                   src/rt_daq/port/result.c
                   src/rt_daq/port.c
                   src/rt_daq/daq.c)

set(RT_DAQ_DRIVER "COMEDI" CACHE STRING "RT DAQ Driver")
set_property(CACHE RT_DAQ_DRIVER PROPERTY STRINGS COMEDI)

if(RT_DAQ_DRIVER STREQUAL "COMEDI")
    find_library(COMEDI_LIBRARY NAMES comedi REQUIRED)
    target_sources(rt_daq PRIVATE
        src/rt_daq/driver/comedi.c)
    target_compile_definitions(rt_daq PRIVATE COMEDI_DRIVER)
    target_link_libraries(rt_daq PUBLIC ${COMEDI_LIBRARY})
else()
    message(FATAL_ERROR "Invalid RT_DAQ_DRIVER")
endif()

target_include_directories(rt_daq PUBLIC include/)

###########
# RT SPSC #
###########

add_library(rt_spsc
            STATIC src/rt_spsc/spsc_command.c
                   src/rt_spsc/spsc_telemetry.c)
 
target_include_directories(rt_spsc PUBLIC include/)


#############
# RT Plugin #
#############

add_library(rt_plugin
            STATIC src/rt_plugin/abi/result.c
                   src/rt_plugin/plugin.c)
 
target_include_directories(rt_plugin PUBLIC include/)

#############
#   Main    #
#############

add_executable(c_rt_comunication)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_result)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_thread)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_daq)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_spsc)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_plugin)

target_sources(c_rt_comunication
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

################
#   RT Tests    #
################

enable_testing()
add_subdirectory(test)
