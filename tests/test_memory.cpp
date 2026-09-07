#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include "emulator/core/memory/MemoryManager.h"

using emu::memory::MemoryError;
using emu::memory::MemoryManager;

TEST_CASE("MemoryManager configures main memory size") {
    MemoryManager memory(1024);
    REQUIRE(memory.size() == 1024);
    REQUIRE(memory.last_error() == MemoryError::None);
}

TEST_CASE("MemoryManager reads and writes bytes") {
    MemoryManager memory(16);

    REQUIRE(memory.Write8(0, 0xAB));
    REQUIRE(memory.Read8(0) == 0xAB);
    REQUIRE(memory.Write8(15, 0xCD));
    REQUIRE(memory.Read8(15) == 0xCD);
}

TEST_CASE("MemoryManager uses PowerPC big-endian 16-bit ordering") {
    MemoryManager memory(8);

    REQUIRE(memory.Write16(2, 0x1234));
    REQUIRE(memory.data()[2] == 0x12);
    REQUIRE(memory.data()[3] == 0x34);
    REQUIRE(memory.Read16(2) == 0x1234);
}

TEST_CASE("MemoryManager uses PowerPC big-endian 32-bit ordering") {
    MemoryManager memory(8);

    REQUIRE(memory.Write32(1, 0x12345678));
    REQUIRE(memory.data()[1] == 0x12);
    REQUIRE(memory.data()[2] == 0x34);
    REQUIRE(memory.data()[3] == 0x56);
    REQUIRE(memory.data()[4] == 0x78);
    REQUIRE(memory.Read32(1) == 0x12345678);
}

TEST_CASE("MemoryManager uses PowerPC big-endian 64-bit ordering") {
    MemoryManager memory(16);

    REQUIRE(memory.Write64(0, 0x0123456789ABCDEFULL));
    REQUIRE(memory.data()[0] == 0x01);
    REQUIRE(memory.data()[1] == 0x23);
    REQUIRE(memory.data()[2] == 0x45);
    REQUIRE(memory.data()[3] == 0x67);
    REQUIRE(memory.data()[4] == 0x89);
    REQUIRE(memory.data()[5] == 0xAB);
    REQUIRE(memory.data()[6] == 0xCD);
    REQUIRE(memory.data()[7] == 0xEF);
    REQUIRE(memory.Read64(0) == 0x0123456789ABCDEFULL);
}

TEST_CASE("MemoryManager accepts unaligned accesses at the byte abstraction") {
    MemoryManager memory(16);

    REQUIRE(memory.Write32(3, 0xDEADBEEF));
    REQUIRE(memory.Read32(3) == 0xDEADBEEF);
}

TEST_CASE("MemoryManager rejects out-of-bounds accesses") {
    MemoryManager memory(8);

    REQUIRE_FALSE(memory.Write8(8, 0xFF));
    REQUIRE(memory.last_error() == MemoryError::AddressOutOfRange);
    REQUIRE_FALSE(memory.last_error_message().empty());
    REQUIRE(memory.Read8(8) == 0);
    REQUIRE(memory.last_error() == MemoryError::AddressOutOfRange);

    REQUIRE_FALSE(memory.Write16(7, 0x1234));
    REQUIRE_FALSE(memory.Write32(5, 0x12345678));
    REQUIRE_FALSE(memory.Write64(1, 0x123456789ABCDEF0ULL));
}

TEST_CASE("MemoryManager accepts exact end boundaries") {
    MemoryManager memory(8);

    REQUIRE(memory.Write16(6, 0xBEEF));
    REQUIRE(memory.Read16(6) == 0xBEEF);
    REQUIRE(memory.Write32(4, 0x12345678));
    REQUIRE(memory.Read32(4) == 0x12345678);
    REQUIRE(memory.Write64(0, 0x0123456789ABCDEFULL));
    REQUIRE(memory.Read64(0) == 0x0123456789ABCDEFULL);
}

TEST_CASE("MemoryManager reset clears memory and errors") {
    MemoryManager memory(32);

    REQUIRE(memory.Write64(8, 0x0123456789ABCDEFULL));
    REQUIRE_FALSE(memory.Write32(30, 1));
    REQUIRE(memory.last_error() == MemoryError::AddressOutOfRange);

    memory.Reset();

    REQUIRE(memory.last_error() == MemoryError::None);
    REQUIRE(memory.last_error_message().empty());
    REQUIRE(memory.Read8(8) == 0);
    REQUIRE(memory.Read64(8) == 0);
    REQUIRE(memory.last_error() == MemoryError::None);
}

TEST_CASE("MemoryManager reports range validity without modifying state") {
    MemoryManager memory(16);

    REQUIRE(memory.IsRangeValid(0, 16));
    REQUIRE(memory.IsRangeValid(15, 1));
    REQUIRE_FALSE(memory.IsRangeValid(15, 2));
    REQUIRE_FALSE(memory.IsRangeValid(16, 1));
    REQUIRE(memory.IsRangeValid(16, 0));
}
