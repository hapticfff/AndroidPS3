#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include "emulator/core/cpu/ppu/PPUDecoder.h"
#include "emulator/core/cpu/ppu/PPUInterpreter.h"
using namespace emu::cpu::ppu;
static std::uint32_t D(std::uint32_t op,std::uint32_t rt,std::uint32_t ra,std::int16_t imm){return(op<<26)|(rt<<21)|(ra<<16)|static_cast<std::uint16_t>(imm);}
static std::uint32_t X(std::uint32_t rt,std::uint32_t ra,std::uint32_t rb,std::uint32_t xo,bool rc=false){return(31u<<26)|(rt<<21)|(ra<<16)|(rb<<11)|(xo<<1)|(rc?1u:0u);}
static std::uint32_t B(std::int32_t off,bool lk=false){return(18u<<26)|((static_cast<std::uint32_t>(off>>2)&0xffffffu)<<2)|(lk?1u:0u);}
TEST_CASE("PPU NOP") { PPUDecoder d; REQUIRE(d.Decode(0x60000000u).opcode==PPUOpcode::Nop); }
TEST_CASE("PPU ADDI sign extension") { PPUInterpreter p; p.state().set_pc(0x1000); REQUIRE(p.Step(D(14,3,0,-5))==PPUExecutionResult::Executed); REQUIRE(p.state().Gpr(3)==0xfffffffffffffffbULL); REQUIRE(p.state().pc()==0x1004); }
TEST_CASE("PPU ADD") { PPUInterpreter p; p.state().Gpr(4)=10;p.state().Gpr(5)=32;p.Step(X(3,4,5,266));REQUIRE(p.state().Gpr(3)==42); }
TEST_CASE("PPU SUBF") { PPUInterpreter p;p.state().Gpr(4)=10;p.state().Gpr(5)=32;p.Step(X(3,4,5,40));REQUIRE(p.state().Gpr(3)==22); }
TEST_CASE("PPU logical operations") { PPUInterpreter p;p.state().Gpr(4)=0xf0f0;p.state().Gpr(5)=0x0ff0;p.Step(X(3,4,5,444));REQUIRE(p.state().Gpr(3)==0xfff0);p.Step(X(3,4,5,28));REQUIRE(p.state().Gpr(3)==0x00f0);p.Step(X(3,4,5,316));REQUIRE(p.state().Gpr(3)==0xff00); }
TEST_CASE("PPU record form updates CR0") { PPUInterpreter p;p.state().Gpr(4)=2;p.state().Gpr(5)=3;p.Step(X(3,4,5,266,true));REQUIRE((p.state().condition_register()>>28)==4u); }
TEST_CASE("PPU branch and link") { PPUInterpreter p;p.state().set_pc(0x1000);p.Step(B(0x20));REQUIRE(p.state().pc()==0x1020);p.state().set_pc(0x2000);p.Step(B(-0x10,true));REQUIRE(p.state().pc()==0x1ff0);REQUIRE(p.state().link_register()==0x2004); }
TEST_CASE("PPU unsupported instruction") { PPUInterpreter p;REQUIRE(p.Step(0xffffffffu)==PPUExecutionResult::UnsupportedInstruction);REQUIRE_FALSE(p.last_error().empty()); }
TEST_CASE("PPU reset") { PPUInterpreter p;p.state().Gpr(7)=123;p.state().set_pc(0x5000);p.Reset();REQUIRE(p.state().Gpr(7)==0);REQUIRE(p.state().pc()==0); }
