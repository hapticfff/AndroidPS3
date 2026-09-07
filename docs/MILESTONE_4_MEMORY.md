# Milestone 4 — Memory Subsystem

This milestone introduces the first real emulated memory abstraction used by the PPU foundation.

Scope:
- 32-bit address abstraction
- Configurable main-memory size
- 8/16/32/64-bit reads and writes
- PowerPC big-endian byte ordering
- Bounds checking and explicit errors
- Deterministic reset/initialization

This milestone does not implement SPU, RSX, Vulkan, JIT, firmware, or game compatibility.
