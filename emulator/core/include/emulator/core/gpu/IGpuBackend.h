#pragma once

#include <string>

namespace emu::gpu {

enum class BackendState { Uninitialized, Ready, Running, Stopped, Error };

class IGpuBackend {
public:
    virtual ~IGpuBackend() = default;
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual BackendState GetState() const = 0;
    virtual std::string GetName() const = 0;
    virtual bool PresentFrame() = 0;
};

} // namespace emu::gpu
