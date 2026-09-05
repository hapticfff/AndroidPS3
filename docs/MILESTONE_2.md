# Milestone 2 — PPU Foundation

The repository now contains a small, testable PowerPC PPU interpreter foundation.

Implemented: architectural register state, instruction representation/decoding, ADDI, ADD, SUBF, OR, AND, XOR, relative B with optional link, CR0 record forms, explicit unsupported-instruction errors, and native unit tests.

Not implemented yet: complete PowerPC instruction coverage, floating point, VMX/AltiVec, exceptions/interrupts, memory/load-store, JIT, SPU, RSX/Vulkan, and PS3 OS/HLE.

Before calling this milestone Android-build verified, the Gradle wrapper JAR and direct PPU-to-EmulatorCore integration must be completed and CI must pass.