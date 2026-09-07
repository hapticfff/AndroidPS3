#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace emu::memory {

enum class MemoryError {
    None,
    AddressOutOfRange,
};

class MemoryManager {
public:
    static constexpr std::uint64_t kAddressSpaceSize = 0x1'0000'0000ULL;
    static constexpr std::size_t kDefaultMainMemorySize = 256ULL * 1024ULL * 1024ULL;

    explicit MemoryManager(std::size_t main_memory_size = kDefaultMainMemorySize);

    void Reset();
    std::size_t size() const noexcept { return data_.size(); }

    MemoryError last_error() const noexcept { return last_error_; }
    const std::string& last_error_message() const noexcept { return last_error_message_; }
    void ClearError() const noexcept;

    bool IsRangeValid(std::uint32_t address, std::size_t width) const noexcept;

    std::uint8_t Read8(std::uint32_t address) const;
    std::uint16_t Read16(std::uint32_t address) const;
    std::uint32_t Read32(std::uint32_t address) const;
    std::uint64_t Read64(std::uint32_t address) const;

    bool Write8(std::uint32_t address, std::uint8_t value);
    bool Write16(std::uint32_t address, std::uint16_t value);
    bool Write32(std::uint32_t address, std::uint32_t value);
    bool Write64(std::uint32_t address, std::uint64_t value);

    const std::uint8_t* data() const noexcept { return data_.data(); }
    std::uint8_t* data() noexcept { return data_.data(); }

private:
    bool CheckRange(std::uint32_t address, std::size_t width) const noexcept;
    void SetError(MemoryError error, const char* message) const;

    std::vector<std::uint8_t> data_;
    mutable MemoryError last_error_{MemoryError::None};
    mutable std::string last_error_message_{};
};

} // namespace emu::memory
