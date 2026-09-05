#pragma once

#include <cstdint>
#include <string>
#include "emulator/core/cpu/ppu/PPUDecoder.h"
#include "emulator/core/cpu/ppu/PPUState.h"

namespace emu::cpu::ppu {

enum class PPUExecutionResult {
    Executed,
    UnsupportedInstruction
};

class PPUInterpreter {
public:
    PPUInterpreter();

    void Reset();
    PPUExecutionResult Step(std::uint32_t raw);

    PPUState& state() { return state_; }
    const PPUState& state() const { return state_; }
    const std::string& last_error() const { return last_error_; }

private:
    void SetError(const std::string& message);
    void UpdateCR0(std::uint64_t value);

    PPUState state_{};
    PPUDecoder decoder_{};
    std::string last_error_{};
};

} // namespace emu::cpu::ppu
