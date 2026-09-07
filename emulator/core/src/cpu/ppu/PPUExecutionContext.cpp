#include "emulator/core/cpu/ppu/PPUExecutionContext.h"

namespace emu::cpu::ppu {

PPUExecutionResult PPUExecutionContext::Step() {
    last_error_message_ = nullptr;
    const std::uint64_t pc = interpreter_.state().pc();
    if (pc > 0xFFFF'FFFFULL) {
        last_error_message_ = "PPU PC is outside the 32-bit guest address space";
        return PPUExecutionResult::UnsupportedInstruction;
    }

    const std::uint32_t instruction = memory_.Read32(static_cast<std::uint32_t>(pc));
    if (memory_.last_error() != emu::memory::MemoryError::None) {
        last_error_message_ = memory_.last_error_message().c_str();
        return PPUExecutionResult::UnsupportedInstruction;
    }
    return interpreter_.Step(instruction, memory_);
}

} // namespace emu::cpu::ppu
