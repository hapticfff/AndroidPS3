#include "emulator/core/cpu/ppu/PPUInterpreter.h"
#include <sstream>
#include "emulator/logging/Logger.h"

namespace emu::cpu::ppu {
namespace { std::int64_t S64(std::uint64_t v) { return static_cast<std::int64_t>(v); } }

PPUInterpreter::PPUInterpreter() { Reset(); }
void PPUInterpreter::Reset() { state_.Reset(); last_error_.clear(); }
void PPUInterpreter::SetError(const std::string& m) { last_error_ = m; EMU_LOG_ERROR(emu::logging::Category::CPU, m); }
void PPUInterpreter::UpdateCR0(std::uint64_t v) { std::uint32_t n = (S64(v) < 0) ? 8u : (S64(v) > 0) ? 4u : 2u; state_.set_condition_register((state_.condition_register() & 0x0FFFFFFFu) | (n << 28)); }

PPUExecutionResult PPUInterpreter::Step(std::uint32_t raw) {
    last_error_.clear();
    auto i = decoder_.Decode(raw);
    auto pc = state_.pc();
    switch (i.opcode) {
    case PPUOpcode::Nop: state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Addi: state_.Gpr(i.rt) = (i.ra ? state_.Gpr(i.ra) : 0) + static_cast<std::uint64_t>(i.immediate); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Add: state_.Gpr(i.rt) = state_.Gpr(i.ra) + state_.Gpr(i.rb); if (i.record) UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Subf: state_.Gpr(i.rt) = state_.Gpr(i.rb) - state_.Gpr(i.ra); if (i.record) UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Or: state_.Gpr(i.rt) = state_.Gpr(i.ra) | state_.Gpr(i.rb); if (i.record) UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::And: state_.Gpr(i.rt) = state_.Gpr(i.ra) & state_.Gpr(i.rb); if (i.record) UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Xor: state_.Gpr(i.rt) = state_.Gpr(i.ra) ^ state_.Gpr(i.rb); if (i.record) UpdateCR0(state_.Gpr(i.rt)); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Branch:
        if (i.absolute) { SetError("Absolute branch is not implemented"); return PPUExecutionResult::UnsupportedInstruction; }
        if (i.link) state_.set_link_register(pc + 4);
        state_.set_pc(static_cast<std::uint64_t>(S64(pc) + i.immediate)); return PPUExecutionResult::Executed;
    default: break;
    }
    std::ostringstream s; s << "Unsupported PPU instruction 0x" << std::hex << raw; SetError(s.str()); return PPUExecutionResult::UnsupportedInstruction;
}

PPUExecutionResult PPUInterpreter::Step(std::uint32_t raw, emu::memory::MemoryManager& memory) {
    last_error_.clear();
    const auto i = decoder_.Decode(raw);
    const auto pc = state_.pc();
    const auto effective_address = [&]() -> std::uint32_t {
        const std::uint64_t base = i.ra ? state_.Gpr(i.ra) : 0;
        return static_cast<std::uint32_t>(base + static_cast<std::int64_t>(i.immediate));
    };
    const auto memory_error = [&]() {
        SetError(memory.last_error_message());
        return PPUExecutionResult::UnsupportedInstruction;
    };

    switch (i.opcode) {
    case PPUOpcode::Load8: { const auto v = memory.Read8(effective_address()); if (memory.last_error() != emu::memory::MemoryError::None) return memory_error(); state_.Gpr(i.rt) = v; state_.set_pc(pc + 4); return PPUExecutionResult::Executed; }
    case PPUOpcode::Load16: { const auto v = memory.Read16(effective_address()); if (memory.last_error() != emu::memory::MemoryError::None) return memory_error(); state_.Gpr(i.rt) = v; state_.set_pc(pc + 4); return PPUExecutionResult::Executed; }
    case PPUOpcode::Load32: { const auto v = memory.Read32(effective_address()); if (memory.last_error() != emu::memory::MemoryError::None) return memory_error(); state_.Gpr(i.rt) = v; state_.set_pc(pc + 4); return PPUExecutionResult::Executed; }
    case PPUOpcode::Load64: { const auto v = memory.Read64(effective_address()); if (memory.last_error() != emu::memory::MemoryError::None) return memory_error(); state_.Gpr(i.rt) = v; state_.set_pc(pc + 4); return PPUExecutionResult::Executed; }
    case PPUOpcode::Store8: if (!memory.Write8(effective_address(), static_cast<std::uint8_t>(state_.Gpr(i.rt)))) return memory_error(); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Store16: if (!memory.Write16(effective_address(), static_cast<std::uint16_t>(state_.Gpr(i.rt)))) return memory_error(); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Store32: if (!memory.Write32(effective_address(), static_cast<std::uint32_t>(state_.Gpr(i.rt)))) return memory_error(); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    case PPUOpcode::Store64: if (!memory.Write64(effective_address(), state_.Gpr(i.rt))) return memory_error(); state_.set_pc(pc + 4); return PPUExecutionResult::Executed;
    default: return Step(raw);
    }
}

} // namespace emu::cpu::ppu
