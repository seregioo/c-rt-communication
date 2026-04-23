cmake_minimum_required(VERSION 3.23)

project(c_rt_comunication)

add_executable(c_rt_comunication)

target_sources(c_rt_comunication
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)


