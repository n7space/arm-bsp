project(Samv71Matrix VERSION 1.0.0 LANGUAGES C)

add_library(Samv71Matrix STATIC)
target_sources(Samv71Matrix
    PRIVATE     Matrix.c
    PUBLIC      Matrix.h
                MatrixRegisters.h)
target_include_directories(Samv71Matrix
    PUBLIC      ..)
target_link_libraries(Samv71Matrix
    PRIVATE     common_build_options
                bsp_build_options)

set_target_properties(Samv71Matrix PROPERTIES OUTPUT_NAME "matrix")
add_library(SAMV71::Matrix ALIAS Samv71Matrix)
