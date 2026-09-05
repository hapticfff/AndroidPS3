#pragma once
#include <cstdint>
#include "emulator/core/gpu/IGpuBackend.h"
namespace emu::gpu {
class PlaceholderGpu final : public IGpuBackend {
public:
    bool Initialize() override;
    void Shutdown() override;
    BackendState GetState() const override;
    std::string GetName() const override;
    bool PresentFrame() override;
private:
    BackendState state_ = BackendState::Uninitialized;
    std::uint64_t frame_count_ = 0;
};
} // namespace emu::gpu
