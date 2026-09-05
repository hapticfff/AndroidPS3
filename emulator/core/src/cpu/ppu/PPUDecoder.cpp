#include "emulator/core/cpu/ppu/PPUDecoder.h"
namespace emu::cpu::ppu {
namespace {
std::int64_t se16(std::uint32_t v){return static_cast<std::int16_t>(v&0xFFFFu);}
std::int64_t se24(std::uint32_t v){v&=0x00FFFFFFu; if(v&0x00800000u)v|=0xFF000000u; return static_cast<std::int32_t>(v);}
}
PPUInstruction PPUDecoder::Decode(std::uint32_t raw) const {
    PPUInstruction o{}; o.raw=raw; o.rt=(raw>>21)&31; o.ra=(raw>>16)&31; o.rb=(raw>>11)&31;
    switch(raw>>26){
        case 14: o.opcode=PPUOpcode::Addi; o.immediate=se16(raw); break;
        case 18: o.opcode=PPUOpcode::Branch; o.immediate=se24(raw>>2)<<2; o.absolute=(raw&2)!=0; o.link=(raw&1)!=0; break;
        case 24: if(o.rt==0 && o.ra==0 && (raw&0xFFFFu)==0)o.opcode=PPUOpcode::Nop; break;
        case 31:{o.record=(raw&1)!=0; switch((raw>>1)&0x3FFu){case 266:o.opcode=PPUOpcode::Add;break;case 40:o.opcode=PPUOpcode::Subf;break;case 444:o.opcode=PPUOpcode::Or;break;case 28:o.opcode=PPUOpcode::And;break;case 316:o.opcode=PPUOpcode::Xor;break;default:break;}}break;
        default:break;
    }
    return o;
}
} // namespace emu::cpu::ppu
