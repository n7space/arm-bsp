project(Samv71Utils VERSION 1.0.0 LANGUAGES C)

add_library(Samv71Utils STATIC)
target_sources(Samv71Utils
    PRIVATE     ByteFifo.c
    PUBLIC      ByteFifo.h
                Utils.h)
target_include_directories(Samv71Utils
    PUBLIC      ..)
target_link_libraries(Samv71Utils
    PRIVATE     common_build_options
                bsp_build_options)

set_target_properties(Samv71Utils PROPERTIES OUTPUT_NAME "Utils")
add_library(SAMV71::Utils ALIAS Samv71Utils)
