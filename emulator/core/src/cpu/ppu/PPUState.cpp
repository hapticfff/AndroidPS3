#include "emulator/core/cpu/ppu/PPUState.h"
#include <stdexcept>
namespace emu::cpu::ppu {
void PPUState::Reset(){gpr_.fill(0); pc_=0; condition_register_=0; link_register_=0; count_register_=0; machine_state_register_=0;}
std::uint64_t& PPUState::Gpr(std::size_t i){if(i>=kGprCount)throw std::out_of_range("PPU GPR index out of range"); return gpr_[i];}
std::uint64_t PPUState::Gpr(std::size_t i) const{if(i>=kGprCount)throw std::out_of_range("PPU GPR index out of range"); return gpr_[i];}
} // namespace emu::cpu::ppu
