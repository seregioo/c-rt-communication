cmake_minimum_required(VERSION 3.23)

project(c_rt_comunication VERSION 1.0
        DESCRIPTION "RealTime communication c project"
        LANGUAGES C)

add_library(rt_communication_result STATIC src/rt_communication_result/result.c include/rt_communication_result)
target_include_directories(rt_communication_result PUBLIC include)

add_executable(c_rt_comunication)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_communication_result)

target_sources(c_rt_comunication
  PRIVATE
    src/main.c
)


