#include "emulator/core/EmulatorCore.h"

#include <memory>
#include <sstream>

#include "emulator/core/cpu/ppu/PPUBackend.h"
#include "emulator/core/src/gpu/PlaceholderGpu.h"
#include "emulator/logging/Logger.h"

namespace emu::core {
namespace { constexpr const char* kVersionString = "AndroidPS3 0.1.0-m2.1"; }

EmulatorCore::EmulatorCore()
    : cpu_backend_(std::make_unique<emu::cpu::ppu::PPUBackend>()),
      gpu_backend_(std::make_unique<emu::gpu::PlaceholderGpu>()) {}

EmulatorCore::~EmulatorCore() { if (initialized_) Shutdown(); }

bool EmulatorCore::Initialize() {
    if (!cpu_backend_->Initialize()) return false;
    if (!gpu_backend_->Initialize()) { cpu_backend_->Shutdown(); return false; }
    initialized_ = true;
    EMU_LOG_INFO(emu::logging::Category::GENERAL, "EmulatorCore initialized with PPU interpreter");
    return true;
}

void EmulatorCore::Shutdown() {
    gpu_backend_->Shutdown();
    cpu_backend_->Shutdown();
    initialized_ = false;
}

bool EmulatorCore::IsInitialized() const { return initialized_; }

bool EmulatorCore::RunFrame() {
    if (!initialized_) return false;
    return cpu_backend_->Step() && gpu_backend_->PresentFrame();
}

std::string EmulatorCore::GetStatusString() const {
    std::ostringstream out;
    out << "CPU backend: " << cpu_backend_->GetName()
        << " [" << static_cast<int>(cpu_backend_->GetState()) << "]\n"
        << "GPU backend: " << gpu_backend_->GetName()
        << " [" << static_cast<int>(gpu_backend_->GetState()) << "]";
    return out.str();
}

std::string EmulatorCore::GetVersionString() const { return kVersionString; }

} // namespace emu::core
