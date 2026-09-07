#pragma once

#include <cstdint>

#include "emulator/core/cpu/ppu/PPUInterpreter.h"
#include "emulator/core/memory/MemoryManager.h"

namespace emu::cpu::ppu {

// Connects PPU execution state to guest memory. Instruction fetches are
// explicitly big-endian, matching the PowerPC execution model.
class PPUExecutionContext {
public:
    PPUExecutionContext(PPUInterpreter& interpreter, emu::memory::MemoryManager& memory)
        : interpreter_(interpreter), memory_(memory) {}

    PPUExecutionResult Step();

    PPUInterpreter& interpreter() noexcept { return interpreter_; }
    const PPUInterpreter& interpreter() const noexcept { return interpreter_; }
    emu::memory::MemoryManager& memory() noexcept { return memory_; }
    const emu::memory::MemoryManager& memory() const noexcept { return memory_; }

    const char* last_error_message() const noexcept { return last_error_message_; }

private:
    PPUInterpreter& interpreter_;
    emu::memory::MemoryManager& memory_;
    const char* last_error_message_ = nullptr;
};

} // namespace emu::cpu::ppu
