cmake_minimum_required(VERSION 3.22)

add_library(emulator_core STATIC
    src/EmulatorCore.cpp
    src/logging/Logger.cpp
    src/config/Config.cpp
    src/cpu/PlaceholderCpu.cpp
    src/cpu/ppu/PPUState.cpp
    src/cpu/ppu/PPUDecoder.cpp
    src/cpu/ppu/PPUInterpreter.cpp
    src/cpu/ppu/PPUBackend.cpp
    src/gpu/PlaceholderGpu.cpp
)

target_include_directories(emulator_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(emulator_core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(emulator_core PRIVATE /W4)
else()
    target_compile_options(emulator_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
