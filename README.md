# AndroidPS3

Open-source, independently implemented PlayStation 3 emulator targeting modern Android ARM64 devices.

## Current status — Milestone 2

PPU foundation is present: architectural state, instruction decoding, a small deterministic integer/branch interpreter, CR0 record forms, and tests.

No Sony firmware, encryption keys, proprietary source, or copyrighted game data is included. Users provide legally obtained PS3 system software/game files themselves.

### Next engineering step

Wire the PPU interpreter into `EmulatorCore`, complete memory/load-store foundations, and make the Android CI build reproducible with a complete Gradle wrapper.
