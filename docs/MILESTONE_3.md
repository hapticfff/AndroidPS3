# Milestone 3 — Android UI Foundation

## Goal
Build the first real AetherPS3 Android frontend as a landscape-only application, based on the approved AetherPS3 UI direction and kept separate from emulator internals.

## Scope
- Landscape-only Android shell
- Home screen
- Games screen
- Library screen
- Game details screen
- Downloads screen
- Settings navigation
- General, System, Graphics, Audio, Controller, Emulation, Advanced settings screens
- About screen
- Local UI state and interactions
- Screen transitions and lightweight motion
- Responsive layouts for landscape phone/tablet sizes
- Clean NativeBridge boundary; UI must not manipulate PPU internals directly

## Explicitly out of scope
- SPU implementation
- RSX implementation
- Vulkan renderer implementation
- JIT implementation
- PS3 OS/HLE
- Firmware or key handling
- Game compatibility logic
- Real game downloading

## Design direction
- Landscape-first 16:9 composition
- Dark black/blue gaming interface
- Premium console-style hierarchy
- Spacious game cards
- Blue accent for selection/focus/actions
- Smooth but controlled animations
- No dependency on proprietary Sony assets

## Development rules
- Preserve Milestone 2 native tests.
- Keep UI logic in Android/Kotlin and emulator logic in C++.
- Do not replace working native emulator code just to support the UI.
- Use local mock/sample data only for UI prototyping.
- Keep main branch untouched until the milestone is reviewed and CI passes.
