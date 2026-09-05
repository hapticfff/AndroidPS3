cmake_minimum_required(VERSION 3.22)

add_library(androidps3_jni SHARED JniBridge.cpp)
target_link_libraries(androidps3_jni PRIVATE emulator_core log)
target_compile_features(androidps3_jni PRIVATE cxx_std_20)
