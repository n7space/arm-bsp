project(Samv71Mpu VERSION 1.0.0 LANGUAGES C)

add_library(Samv71Mpu STATIC)
target_sources(Samv71Mpu
    PRIVATE     Mpu.c
    PUBLIC      Mpu.h
                MpuRegisters.h)
target_include_directories(Samv71Mpu
    PUBLIC      ..)
target_link_libraries(Samv71Mpu
    PRIVATE     common_build_options
                bsp_build_options)

set_target_properties(Samv71Mpu PROPERTIES OUTPUT_NAME "mpu")
add_library(SAMV71::Mpu ALIAS Samv71Mpu)
