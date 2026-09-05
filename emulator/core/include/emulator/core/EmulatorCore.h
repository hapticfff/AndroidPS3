#pragma once

#include <memory>
#include <string>

#include "emulator/core/cpu/ICpuBackend.h"
#include "emulator/core/gpu/IGpuBackend.h"

namespace emu::core {

class EmulatorCore {
public:
    EmulatorCore();
    ~EmulatorCore();

    bool Initialize();
    void Shutdown();
    bool IsInitialized() const;
    bool RunFrame();
    std::string GetStatusString() const;
    std::string GetVersionString() const;

private:
    std::unique_ptr<emu::cpu::ICpuBackend> cpu_backend_;
    std::unique_ptr<emu::gpu::IGpuBackend> gpu_backend_;
    bool initialized_ = false;
};

} // namespace emu::core
