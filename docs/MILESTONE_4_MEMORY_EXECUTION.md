# Milestone 4 — Memory-backed PPU Execution

## Part 2

The PPU foundation now has a real guest-memory instruction path:

`PC -> MemoryManager::Read32 -> PPU decoder -> PPU interpreter -> next PC`

Instruction fetches use the MemoryManager's PowerPC big-endian 32-bit reads. Fetch failures are surfaced as execution errors instead of executing a fallback instruction.

The PPU backend now owns guest memory and uses the execution context rather than a hardcoded NOP. Tests cover sequential execution, ADDI fetched from memory, branch execution, invalid fetches, and unsupported fetched instructions.

This remains an interpreter-only milestone. It does not implement SPU, RSX, Vulkan, JIT, firmware, or game compatibility.
