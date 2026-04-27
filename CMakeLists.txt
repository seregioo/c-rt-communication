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
            STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/rt_thread/result.c)

target_include_directories(rt_thread PUBLIC include/)

#############
#   Main    #
#############

add_executable(c_rt_comunication)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_result)

target_link_libraries(c_rt_comunication 
                      PUBLIC rt_thread)
                  
target_sources(c_rt_comunication
  PRIVATE
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
