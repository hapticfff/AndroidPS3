#include "emulator/core/cpu/ppu/PPUInterpreter.h"
#include <sstream>
#include "emulator/logging/Logger.h"
namespace emu::cpu::ppu {
namespace { std::int64_t S64(std::uint64_t v){return static_cast<std::int64_t>(v);} }
PPUInterpreter::PPUInterpreter(){Reset();}
void PPUInterpreter::Reset(){state_.Reset(); last_error_.clear();}
void PPUInterpreter::SetError(const std::string& m){last_error_=m; EMU_LOG_ERROR(emu::logging::Category::CPU,m);}
void PPUInterpreter::UpdateCR0(std::uint64_t v){std::uint32_t n=(S64(v)<0)?8u:(S64(v)>0)?4u:2u; state_.set_condition_register((state_.condition_register()&0x0FFFFFFFu)|(n<<28));}
PPUExecutionResult PPUInterpreter::Step(std::uint32_t raw){
    last_error_.clear(); auto i=decoder_.Decode(raw); auto pc=state_.pc();
    switch(i.opcode){
    case PPUOpcode::Nop: state_.set_pc(pc+4); return PPUExecutionResult::Executed;
    case PPUOpcode::Addi: state_.Gpr(i.rt)=(i.ra?state_.Gpr(i.ra):0)+static_cast<std::uint64_t>(i.immediate); state_.set_pc(pc+4); return PPUExecutionResult::Executed;
    case PPUOpcode::Add: state_.Gpr(i.rt)=state_.Gpr(i.ra)+state_.Gpr(i.rb); if(i.record)UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc+4); return PPUExecutionResult::Executed;
    case PPUOpcode::Subf: state_.Gpr(i.rt)=state_.Gpr(i.rb)-state_.Gpr(i.ra); if(i.record)UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc+4); return PPUExecutionResult::Executed;
    case PPUOpcode::Or: state_.Gpr(i.rt)=state_.Gpr(i.ra)|state_.Gpr(i.rb); if(i.record)UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc+4); return PPUExecutionResult::Executed;
    case PPUOpcode::And: state_.Gpr(i.rt)=state_.Gpr(i.ra)&state_.Gpr(i.rb); if(i.record)UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc+4); return PPUExecutionResult::Executed;
    case PPUOpcode::Xor: state_.Gpr(i.rt)=state_.Gpr(i.ra)^state_.Gpr(i.rb); if(i.record)UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc+4); return PPUExecutionResult::Executed;
    case PPUOpcode::Branch:
        if(i.absolute){SetError("Absolute branch is not implemented in milestone 2.1"); return PPUExecutionResult::UnsupportedInstruction;}
        if(i.link)state_.set_link_register(pc+4); state_.set_pc(static_cast<std::uint64_t>(S64(pc)+i.immediate)); return PPUExecutionResult::Executed;
    case PPUOpcode::Invalid: break;
    }
    std::ostringstream s; s<<"Unsupported PPU instruction 0x"<<std::hex<<raw; SetError(s.str()); return PPUExecutionResult::UnsupportedInstruction;
}
} // namespace emu::cpu::ppu
