#pragma once

#include <cstdint>

namespace emu::cpu::ppu {

enum class PPUOpcode : std::uint8_t {
    Invalid, Nop, Addi, Add, Subf, Or, And, Xor, Branch,
    Load8, Load16, Load32, Load64, Store8, Store16, Store32, Store64
};

struct PPUInstruction {
    PPUOpcode opcode = PPUOpcode::Invalid;
    std::uint32_t raw = 0;
    std::uint8_t rt = 0, ra = 0, rb = 0;
    std::int64_t immediate = 0;
    bool record = false, link = false, absolute = false;
};

} // namespace emu::cpu::ppu
