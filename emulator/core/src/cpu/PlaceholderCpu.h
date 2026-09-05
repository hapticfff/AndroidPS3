#pragma once
#include "emulator/core/cpu/ICpuBackend.h"
namespace emu::cpu {
class PlaceholderCpu final : public ICpuBackend {
public:
    bool Initialize() override;
    void Shutdown() override;
    BackendState GetState() const override;
    std::string GetName() const override;
    bool Step() override;
private:
    BackendState state_ = BackendState::Uninitialized;
};
} // namespace emu::cpu
