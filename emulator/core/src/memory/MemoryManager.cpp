#include "emulator/core/memory/MemoryManager.h"

#include <algorithm>
#include <cstring>
#include <limits>

namespace emu::memory {

MemoryManager::MemoryManager(std::size_t main_memory_size)
    : data_(main_memory_size, 0) {}

void MemoryManager::Reset() {
    std::fill(data_.begin(), data_.end(), std::uint8_t{0});
    ClearError();
}

void MemoryManager::ClearError() const noexcept {
    last_error_ = MemoryError::None;
    last_error_message_.clear();
}

bool MemoryManager::IsRangeValid(std::uint32_t address, std::size_t width) const noexcept {
    if (width == 0) {
        return true;
    }

    const std::uint64_t start = address;
    const std::uint64_t end_exclusive = start + width;
    return end_exclusive <= data_.size() && end_exclusive <= kAddressSpaceSize;
}

bool MemoryManager::CheckRange(std::uint32_t address, std::size_t width) const noexcept {
    if (IsRangeValid(address, width)) {
        return true;
    }
    SetError(MemoryError::AddressOutOfRange, "memory access is outside the configured address range");
    return false;
}

void MemoryManager::SetError(MemoryError error, const char* message) const {
    last_error_ = error;
    last_error_message_ = message;
}

std::uint8_t MemoryManager::Read8(std::uint32_t address) const {
    if (!CheckRange(address, 1)) {
        return 0;
    }
    ClearError();
    return data_[address];
}

std::uint16_t MemoryManager::Read16(std::uint32_t address) const {
    if (!CheckRange(address, 2)) {
        return 0;
    }
    ClearError();
    return (static_cast<std::uint16_t>(data_[address]) << 8) |
           static_cast<std::uint16_t>(data_[address + 1]);
}

std::uint32_t MemoryManager::Read32(std::uint32_t address) const {
    if (!CheckRange(address, 4)) {
        return 0;
    }
    ClearError();
    return (static_cast<std::uint32_t>(data_[address]) << 24) |
           (static_cast<std::uint32_t>(data_[address + 1]) << 16) |
           (static_cast<std::uint32_t>(data_[address + 2]) << 8) |
           static_cast<std::uint32_t>(data_[address + 3]);
}

std::uint64_t MemoryManager::Read64(std::uint32_t address) const {
    if (!CheckRange(address, 8)) {
        return 0;
    }
    ClearError();
    std::uint64_t value = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        value = (value << 8) | static_cast<std::uint64_t>(data_[address + i]);
    }
    return value;
}

bool MemoryManager::Write8(std::uint32_t address, std::uint8_t value) {
    if (!CheckRange(address, 1)) {
        return false;
    }
    data_[address] = value;
    ClearError();
    return true;
}

bool MemoryManager::Write16(std::uint32_t address, std::uint16_t value) {
    if (!CheckRange(address, 2)) {
        return false;
    }
    data_[address] = static_cast<std::uint8_t>(value >> 8);
    data_[address + 1] = static_cast<std::uint8_t>(value);
    ClearError();
    return true;
}

bool MemoryManager::Write32(std::uint32_t address, std::uint32_t value) {
    if (!CheckRange(address, 4)) {
        return false;
    }
    data_[address] = static_cast<std::uint8_t>(value >> 24);
    data_[address + 1] = static_cast<std::uint8_t>(value >> 16);
    data_[address + 2] = static_cast<std::uint8_t>(value >> 8);
    data_[address + 3] = static_cast<std::uint8_t>(value);
    ClearError();
    return true;
}

bool MemoryManager::Write64(std::uint32_t address, std::uint64_t value) {
    if (!CheckRange(address, 8)) {
        return false;
    }
    for (std::size_t i = 0; i < 8; ++i) {
        const std::size_t shift = (7 - i) * 8;
        data_[address + i] = static_cast<std::uint8_t>(value >> shift);
    }
    ClearError();
    return true;
}

} // namespace emu::memory
