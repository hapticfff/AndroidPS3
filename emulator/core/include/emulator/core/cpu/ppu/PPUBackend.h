#pragma once

#include "emulator/core/cpu/ICpuBackend.h"
#include "emulator/core/cpu/ppu/PPUExecutionContext.h"
#include "emulator/core/cpu/ppu/PPUInterpreter.h"
#include "emulator/core/memory/MemoryManager.h"

namespace emu::cpu::ppu {

// ICpuBackend adapter for the PPU interpreter and its guest-memory execution path.
class PPUBackend final : public emu::cpu::ICpuBackend {
public:
    bool Initialize() override;
    void Shutdown() override;

    emu::cpu::BackendState GetState() const override;
    std::string GetName() const override;
    bool Step() override;

    PPUInterpreter& interpreter() noexcept { return interpreter_; }
    const PPUInterpreter& interpreter() const noexcept { return interpreter_; }
    emu::memory::MemoryManager& memory() noexcept { return memory_; }
    const emu::memory::MemoryManager& memory() const noexcept { return memory_; }

private:
    emu::cpu::BackendState state_ = emu::cpu::BackendState::Uninitialized;
    emu::memory::MemoryManager memory_{};
    PPUInterpreter interpreter_{};
    PPUExecutionContext execution_context_{interpreter_, memory_};
};

} // namespace emu::cpu::ppu
