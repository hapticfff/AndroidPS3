#include "PlaceholderGpu.h"
namespace emu::gpu {
bool PlaceholderGpu::Initialize(){state_=BackendState::Ready; frame_count_=0; return true;}
void PlaceholderGpu::Shutdown(){state_=BackendState::Stopped;}
BackendState PlaceholderGpu::GetState() const{return state_;}
std::string PlaceholderGpu::GetName() const{return "Legacy Placeholder GPU";}
bool PlaceholderGpu::PresentFrame(){if(state_!=BackendState::Ready && state_!=BackendState::Running)return false; state_=BackendState::Running; ++frame_count_; return true;}
} // namespace emu::gpu
