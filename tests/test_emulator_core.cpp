#include <catch2/catch_test_macros.hpp>
#include "emulator/core/EmulatorCore.h"

TEST_CASE("EmulatorCore starts uninitialized") {
    emu::core::EmulatorCore core;
    REQUIRE_FALSE(core.IsInitialized());
}

TEST_CASE("EmulatorCore initializes with PPU backend") {
    emu::core::EmulatorCore core;
    REQUIRE(core.Initialize());
    REQUIRE(core.IsInitialized());
    REQUIRE(core.GetStatusString().find("PPU Interpreter") != std::string::npos);
}

TEST_CASE("EmulatorCore RunFrame fails before Initialize") {
    emu::core::EmulatorCore core;
    REQUIRE_FALSE(core.RunFrame());
}

TEST_CASE("EmulatorCore RunFrame succeeds after Initialize") {
    emu::core::EmulatorCore core;
    REQUIRE(core.Initialize());
    REQUIRE(core.RunFrame());
    REQUIRE(core.RunFrame());
}

TEST_CASE("EmulatorCore shuts down cleanly") {
    emu::core::EmulatorCore core;
    REQUIRE(core.Initialize());
    core.Shutdown();
    REQUIRE_FALSE(core.IsInitialized());
}

TEST_CASE("EmulatorCore version is reported") {
    emu::core::EmulatorCore core;
    REQUIRE(core.GetVersionString() == "AndroidPS3 0.1.0-m2.1");
}
