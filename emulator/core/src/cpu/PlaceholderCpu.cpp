#include "PlaceholderCpu.h"
#include "emulator/logging/Logger.h"
namespace emu::cpu {
bool PlaceholderCpu::Initialize(){ state_=BackendState::Ready; return true; }
void PlaceholderCpu::Shutdown(){ state_=BackendState::Stopped; }
BackendState PlaceholderCpu::GetState() const{return state_;}
std::string PlaceholderCpu::GetName() const{return "Legacy Placeholder CPU";}
bool PlaceholderCpu::Step(){ if(state_!=BackendState::Ready && state_!=BackendState::Running) return false; state_=BackendState::Running; return true; }
} // namespace emu::cpu
