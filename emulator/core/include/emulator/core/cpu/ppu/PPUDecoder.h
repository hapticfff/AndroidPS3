#pragma once

#include <cstdint>
#include "emulator/core/cpu/ppu/PPUInstruction.h"

namespace emu::cpu::ppu {

class PPUDecoder {
public:
    PPUInstruction Decode(std::uint32_t raw) const;
};

} // namespace emu::cpu::ppu
