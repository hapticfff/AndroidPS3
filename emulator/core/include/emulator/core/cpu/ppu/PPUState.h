#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
namespace emu::cpu::ppu { class PPUState { public: static constexpr std::size_t kGprCount=32; void Reset(); std::uint64_t& Gpr(std::size_t); std::uint64_t Gpr(std::size_t) const; std::uint64_t pc()const{return pc_;} void set_pc(std::uint64_t v){pc_=v;} std::uint32_t condition_register()const{return cr_;} void set_condition_register(std::uint32_t v){cr_=v;} std::uint64_t link_register()const{return lr_;} void set_link_register(std::uint64_t v){lr_=v;} std::uint64_t count_register()const{return ctr_;} void set_count_register(std::uint64_t v){ctr_=v;} std::uint64_t machine_state_register()const{return msr_;} void set_machine_state_register(std::uint64_t v){msr_=v;} private: std::array<std::uint64_t,kGprCount> gpr_{}; std::uint64_t pc_=0,lr_=0,ctr_=0,msr_=0; std::uint32_t cr_=0; }; }
