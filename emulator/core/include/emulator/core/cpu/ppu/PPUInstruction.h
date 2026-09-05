#pragma once
#include <cstdint>
namespace emu::cpu::ppu { enum class PPUOpcode:std::uint8_t { Invalid,Nop,Addi,Add,Subf,Or,And,Xor,Branch }; struct PPUInstruction { PPUOpcode opcode=PPUOpcode::Invalid; std::uint32_t raw=0; std::uint8_t rt=0,ra=0,rb=0; std::int64_t immediate=0; bool record=false,link=false,absolute=false; }; }
