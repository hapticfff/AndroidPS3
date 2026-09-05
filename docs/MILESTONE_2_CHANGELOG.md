# Milestone 2 Changes

Added the first testable PPU foundation:
- 32 general-purpose registers with architectural state
- PC, CR, LR, CTR and machine-state placeholder
- Instruction representation and decoder
- ADDI, ADD, SUBF, OR, AND, XOR and relative B
- CR0 record-form updates
- Explicit unsupported-instruction handling
- Native unit tests

Known follow-up: wire the PPU interpreter into EmulatorCore and restore a complete Android Gradle wrapper/CI APK build before treating the project as a verified Android build.