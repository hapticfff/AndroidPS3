# Milestone 4 — Memory Subsystem

Milestone 4 introduces the first concrete emulated memory layer used by the PPU foundation.

## Scope

- 32-bit address-space model with a configurable main-memory backing size.
- 8/16/32/64-bit reads and writes.
- PowerPC big-endian byte ordering.
- Explicit bounds validation and error state.
- Deterministic zero-initialization and reset.
- Native tests covering widths, byte order, boundaries, unaligned accesses, reset, and invalid accesses.

## Address model

The class models a 32-bit guest address space. The current backing store represents the configured main-memory region starting at guest address `0`; unmapped addresses are rejected. This is intentionally narrower than a full PlayStation 3 memory map and provides a clean base for later mapped-region/interrupt/MMIO work.

## Error behavior

Reads from invalid addresses return zero and set `MemoryError::AddressOutOfRange`. Writes return `false` and set the same error. Successful operations clear the previous error state.

No SPU, RSX, Vulkan, JIT, firmware, or game-compatibility code is part of this milestone.
