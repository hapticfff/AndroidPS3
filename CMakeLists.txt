cmake_minimum_required(VERSION 3.22)
project(AndroidPS3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(ANDROIDPS3_BUILD_TESTS "Build unit tests" ON)

add_subdirectory(emulator/core)

if(ANDROID)
    add_subdirectory(emulator/jni)
endif()

if(ANDROIDPS3_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
