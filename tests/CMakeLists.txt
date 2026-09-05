cmake_minimum_required(VERSION 3.22)

include(FetchContent)
FetchContent_Declare(catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.5.3)
FetchContent_MakeAvailable(catch2)

add_executable(androidps3_tests
    test_emulator_core.cpp
    test_ppu.cpp
)

target_link_libraries(androidps3_tests PRIVATE emulator_core Catch2::Catch2WithMain)
target_compile_features(androidps3_tests PRIVATE cxx_std_20)

include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(androidps3_tests)
