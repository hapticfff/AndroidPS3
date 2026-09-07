#include <catch2/catch_test_macros.hpp>
#include <cstdint>

#include "emulator/core/cpu/ppu/PPUExecutionContext.h"

using namespace emu::cpu::ppu;
using emu::memory::MemoryManager;

static std::uint32_t D(std::uint32_t op, std::uint32_t rt, std::uint32_t ra, std::int16_t imm) {
    return (op << 26) | (rt << 21) | (ra << 16) | static_cast<std::uint16_t>(imm);
}

TEST_CASE("PPU stores and loads 8, 16, 32 and 64-bit values") {
    MemoryManager memory(0x200);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);

    interpreter.state().Gpr(3) = 0x1122334455667788ULL;
    REQUIRE(memory.Write32(0, D(38, 3, 0, 0x40))); // stb
    REQUIRE(memory.Write32(4, D(44, 3, 0, 0x42))); // sth
    REQUIRE(memory.Write32(8, D(36, 3, 0, 0x44))); // stw
    REQUIRE(memory.Write32(12, D(62, 3, 0, 0x48))); // std
    REQUIRE(memory.Write32(16, D(34, 4, 0, 0x40))); // lbz
    REQUIRE(memory.Write32(20, D(40, 5, 0, 0x42))); // lhz
    REQUIRE(memory.Write32(24, D(32, 6, 0, 0x44))); // lwz
    REQUIRE(memory.Write32(28, D(58, 7, 0, 0x48))); // ld

    for (int i = 0; i < 8; ++i) REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(interpreter.state().Gpr(4) == 0x88);
    REQUIRE(interpreter.state().Gpr(5) == 0x7788);
    REQUIRE(interpreter.state().Gpr(6) == 0x55667788);
    REQUIRE(interpreter.state().Gpr(7) == 0x1122334455667788ULL);
}

TEST_CASE("PPU load/store use RA plus signed immediate") {
    MemoryManager memory(0x200);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);

    interpreter.state().Gpr(3) = 0x80;
    interpreter.state().Gpr(4) = 0xAABBCCDD;
    REQUIRE(memory.Write32(0, D(36, 4, 3, -4))); // stw r4,-4(r3)
    REQUIRE(memory.Write32(4, D(32, 5, 3, -4))); // lwz r5,-4(r3)
    REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(interpreter.state().Gpr(5) == 0xAABBCCDD);
    REQUIRE(memory.Read32(0x7C) == 0xAABBCCDD);
}

TEST_CASE("PPU load/store propagate memory bounds errors") {
    MemoryManager memory(0x80);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);
    interpreter.state().Gpr(3) = 0x7F;
    interpreter.state().Gpr(4) = 1;
    REQUIRE(memory.Write32(0, D(36, 4, 3, 0))); // stw at 0x7f is out of range
    REQUIRE(context.Step() == PPUExecutionResult::UnsupportedInstruction);
    REQUIRE(interpreter.state().pc() == 0);
    REQUIRE(memory.last_error() == emu::memory::MemoryError::AddressOutOfRange);
}
