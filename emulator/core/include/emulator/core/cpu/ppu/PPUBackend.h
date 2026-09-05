#pragma once

#include "emulator/core/cpu/ICpuBackend.h"
#include "emulator/core/cpu/ppu/PPUInterpreter.h"

namespace emu::cpu::ppu {

// ICpuBackend adapter for the PPU interpreter. It provides the stable CPU
// backend seam used by EmulatorCore while keeping PPU details out of Android/JNI.
class PPUBackend final : public emu::cpu::ICpuBackend {
public:
    bool Initialize() override;
    void Shutdown() override;

    emu::cpu::BackendState GetState() const override;
    std::string GetName() const override;
    bool Step() override;

    PPUInterpreter& interpreter() { return interpreter_; }
    const PPUInterpreter& interpreter() const { return interpreter_; }

private:
    emu::cpu::BackendState state_ = emu::cpu::BackendState::Uninitialized;
    PPUInterpreter interpreter_;
};

} // namespace emu::cpu::ppu
