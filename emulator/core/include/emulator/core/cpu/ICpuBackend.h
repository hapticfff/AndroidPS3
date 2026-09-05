#pragma once

#include <string>

namespace emu::cpu {

enum class BackendState { Uninitialized, Ready, Running, Stopped, Error };

class ICpuBackend {
public:
    virtual ~ICpuBackend() = default;
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual BackendState GetState() const = 0;
    virtual std::string GetName() const = 0;
    virtual bool Step() = 0;
};

} // namespace emu::cpu
