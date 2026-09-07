#include "emulator/core/cpu/ppu/PPUBackend.h"

#include "emulator/logging/Logger.h"

namespace emu::cpu::ppu {

bool PPUBackend::Initialize() {
    memory_.Reset();
    interpreter_.Reset();
    state_ = emu::cpu::BackendState::Ready;
    EMU_LOG_INFO(emu::logging::Category::CPU, "PPUBackend::Initialize");
    return true;
}

void PPUBackend::Shutdown() {
    state_ = emu::cpu::BackendState::Stopped;
    EMU_LOG_INFO(emu::logging::Category::CPU, "PPUBackend::Shutdown");
}

emu::cpu::BackendState PPUBackend::GetState() const {
    return state_;
}

std::string PPUBackend::GetName() const {
    return "PPU Interpreter";
}

bool PPUBackend::Step() {
    if (state_ != emu::cpu::BackendState::Ready &&
        state_ != emu::cpu::BackendState::Running) {
        return false;
    }

    state_ = emu::cpu::BackendState::Running;
    const auto result = execution_context_.Step();
    if (result != PPUExecutionResult::Executed) {
        state_ = emu::cpu::BackendState::Error;
        EMU_LOG_ERROR(emu::logging::Category::CPU, "PPUBackend failed to fetch or execute instruction");
        return false;
    }
    return true;
}

} // namespace emu::cpu::ppu
