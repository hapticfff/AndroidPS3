#include <catch2/catch_test_macros.hpp>
#include <cstdint>

#include "emulator/core/cpu/ppu/PPUExecutionContext.h"
#include "emulator/core/cpu/ppu/PPUInterpreter.h"
#include "emulator/core/memory/MemoryManager.h"

using namespace emu::cpu::ppu;
using emu::memory::MemoryManager;

static std::uint32_t D(std::uint32_t op, std::uint32_t rt, std::uint32_t ra, std::int16_t imm) {
    return (op << 26) | (rt << 21) | (ra << 16) | static_cast<std::uint16_t>(imm);
}

static std::uint32_t B(std::int32_t off, bool lk = false) {
    return (18u << 26) | ((static_cast<std::uint32_t>(off >> 2) & 0xffffffu) << 2) | (lk ? 1u : 0u);
}

TEST_CASE("PPU fetches ADDI from guest memory") {
    MemoryManager memory(0x100);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);
    REQUIRE(memory.Write32(0, D(14, 3, 0, 42)));
    REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(interpreter.state().Gpr(3) == 42);
    REQUIRE(interpreter.state().pc() == 4);
}

TEST_CASE("PPU executes sequential instructions from memory") {
    MemoryManager memory(0x100);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);
    REQUIRE(memory.Write32(0, D(14, 3, 0, 10)));
    REQUIRE(memory.Write32(4, D(14, 4, 3, 5)));
    REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(interpreter.state().Gpr(4) == 15);
    REQUIRE(interpreter.state().pc() == 8);
}

TEST_CASE("PPU fetches and executes branch from memory") {
    MemoryManager memory(0x100);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);
    REQUIRE(memory.Write32(0, B(8)));
    REQUIRE(memory.Write32(8, D(14, 5, 0, 99)));
    REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(interpreter.state().pc() == 8);
    REQUIRE(context.Step() == PPUExecutionResult::Executed);
    REQUIRE(interpreter.state().Gpr(5) == 99);
}

TEST_CASE("PPU reports instruction fetch outside guest memory") {
    MemoryManager memory(0x100);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);
    interpreter.state().set_pc(0x100);
    REQUIRE(context.Step() == PPUExecutionResult::UnsupportedInstruction);
    REQUIRE(memory.last_error() == emu::memory::MemoryError::AddressOutOfRange);
}

TEST_CASE("PPU reports unsupported fetched instruction") {
    MemoryManager memory(0x100);
    PPUInterpreter interpreter;
    PPUExecutionContext context(interpreter, memory);
    REQUIRE(memory.Write32(0, 0xffffffffu));
    REQUIRE(context.Step() == PPUExecutionResult::UnsupportedInstruction);
    REQUIRE_FALSE(interpreter.last_error().empty());
}
