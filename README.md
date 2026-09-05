# AndroidPS3

An open-source, independently implemented PlayStation 3 emulator targeting modern Android ARM64 devices.

## Status: Milestone 2 — PPU Foundation

Milestone 2 introduces the first small, testable PowerPC PPU interpreter foundation: architectural state, instruction representation/decoding, integer instructions, relative branching, CR0 record forms, and unit tests.

This repository contains no Sony firmware, encryption keys, proprietary source code, or copyrighted game data. Users must provide their own legally obtained PS3 system software and game files.

## Build

Desktop: `cmake -S . -B build -DANDROIDPS3_BUILD_TESTS=ON && cmake --build build && ctest --test-dir build --output-on-failure`

Android requires SDK 34, NDK 26.3.11579264, and Gradle 8.7. The Gradle wrapper JAR is not included yet; this is a known CI follow-up item.

## Next

SPU foundation, memory subsystem, scheduler/timing, then RSX/Vulkan and PS3 OS/HLE in later milestones.
