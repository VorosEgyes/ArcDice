# Changelog

All notable changes to ArcDice will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Detailed assembly guide with HV programming instructions for `RSTDISBL`
  (deferred — see the discussion in the release-readiness audit, the
  ATtiny13A HV fuse programming is documented separately).
- Initial public **v1.0.0** release (this section is collapsed into the
  [1.0.0] entry below once the release tag is pushed).

## [1.0.0] - 2026-07-19

**First public release.** Suitable for small-batch manufacturing of the
ArcDice 7-LED digital dice kit. KiCad `V2` hardware, MIT-licensed
firmware, full bill of materials, host-native unit tests, and CI build.

### Highlights
- **ATtiny13A firmware** with button-wake sleep, BOD disable during
  sleep, edge-accumulated PRNG entropy, and a startup "all-segments-on"
  flash to surface dead-segment faults immediately.
- **KiCad `V2` hardware design** with 5 mm through-hole LEDs, axial
  DIN0207 resistors, USBasp-friendly ISP header, TP4056 Li-ion charger
  module, and 1S battery footprint. PCBWay-ready Gerber archive is
  bundled under `hardware/Gerber/V2/`.
- **Documented `RSTDISBL` trade-off** — PB4 doubles as the roll
  button and the ATtiny13A RESET pin, so a high-voltage programmer is
  required once the button-only variant is programmed.
- **Full [BOM](docs/BOM.md)** with footprints and suggested supplier
  parts (LCSC, Digi-Key, Mouser, TME, Reichelt, AliExpress).
- **GitHub Actions CI** (`Build & Test` workflow): host-native Unity
  tests gate every AVR build, the firmware size check fails if the
  `.text` section exceeds the 1024-byte ATtiny13A flash budget, and
  the resulting `firmware.hex` + `firmware.elf` are uploaded as a
  14-day artifact.
- **Refactored face-mask logic** in `include/dice.h` as `constexpr`
  values, so the host test runner can validate face-mapping without an
  AVR toolchain.

### Changed since 0.9.0
- No code changes — the `0.9.0` pre-release is functionally identical to
  `1.0.0`. The version bump reflects the addition of the documentation,
  tests, and CI that 0.9.0 was missing.

### Notes for downstream users
- **Programming**: connect a USBasp, build with `pio run -e attiny13a`,
  upload with `pio run -e attiny13a -t upload`. The shipped firmware
  assumes the `RSTDISBL` fuse is already programmed (see README for
  the trade-off and HV programming alternatives).
- **Battery**: use a protected 1S Li-ion/LiPo cell. The TP4056 module
  on board is a charger, not a BMS.
- **LEDs**: 5 mm through-hole, any colour. Forward voltage considerations
  for the 220 Ω current-limiting resistors are in `docs/BOM.md`.

## [0.9.0] - 2026-07-18

First pre-release candidate suitable for small-batch manufacturing (KiCad `V2`).

### Added
- **7-segment dice face documentation**: full table of 1–6 pip layouts, with
  explicit description of which segments form each face.
- **RSTDISBL trade-off warning** in the README and as a top-of-file comment in
  `src/main.cpp`. PB4 (button input) is the ATtiny13A RESET pin; using it
  requires the `RSTDISBL` fuse to be programmed, which **disables ISP
  reprogramming** and requires a high-voltage programmer for any further
  firmware changes.
- **`hardware/Gerber/V2/` archive** (PCBWay-ready ZIP: `Archive.zip`,
  106 KB) including F/B copper, mask, paste, silkscreen, edge cuts, NPTH
  and PTH drill files, plus the `job.gbrjob` summary.

### Changed
- **BOD disable ordering** in `goToSleep()`: moved `sleep_bod_disable()` so
  it runs within the 3-cycle window after `sleep_enable()` as required by
  the ATtiny13A datasheet. Saves ~25 µA in sleep mode.
- **Edge entropy accumulation** in the PCINT0 ISR: entropy is now XOR-accumulated
  across multiple edges instead of overwritten on each interrupt. Combined
  with `TCNT0` and `PINB` samples, gives stronger per-press randomness.
- **Startup flash** in `runStartupTest()`: now ends with a 200 ms
  "all-segments-on" flash before clearing the display, so a dead segment or
  swapped wiring is immediately obvious (the 1..6 cycle hides such faults).
- **Explicit sleep sequence** in `goToSleep()`: replaced `sleep_mode()` with
  `sei() → sleep_cpu() → sleep_disable()` to avoid racing with a pending
  PCINT (which would cause immediate wake-up).
- **Repo hygiene**: `.DS_Store`, `hardware/.DS_Store`, and
  `.vscode/settings.json` removed from version control; `.gitignore` extended
  to keep them out (KiCad local artifacts, VS Code cache, macOS Finder
  metadata already covered).

### Hardware (KiCad `V2`)
- 7 through-hole LEDs, 5 mm diameter.
- 7 axial DIN0207 resistors (220 Ω, one per LED).
- 1 through-hole tactile push button (6 mm, 9.5 mm height) for the roll input.
- 1 slide switch (SS12D06 generic) — likely power on/off.
- 1 SPDT switch.
- 1 TP4056 Li-ion/LiPo charging module on board.
- 1 ATtiny13A-P MCU (DIP-8, through-hole, re-programmable via USBasp unless
  `RSTDISBL` is set).
- 1 single-cell battery footprint.
- 4 M2 mounting holes (2.2 mm drill).

## [0.5.0] - 2026-07-15

Hardware visualization pass.

### Added
- 3D top and 3D bottom renders of the PCB under `docs/`.
- Schematic v1 render (`docs/arcdice-schematic-v1.png`).

### Changed
- KiCad PCB layout refined (commit `c4ac4e9`).

## [0.4.0] - 2026-07-14

Hardware and documentation overhaul: project naming, schematic and PCB
files moved into a self-contained `hardware/` folder.

### Added
- Hardware folder structure: `hardware/ArcDice.kicad_pro`, `.kicad_sch`, `.kicad_pcb`.
- Project renamed: `digital-dice` → `ArcDice` (KiCad project files).
- Rendered images: 3D top, 3D bottom, schematic v1 under `docs/`.

### Removed
- Old `docs/kicad-schematic-checklist.md` and `docs/schematic-v1.md`
  (the checklist is now embedded in the README and KiCad files themselves).
- Empty `include/README`, `lib/README`, `test/README` placeholder files
  (the empty folders themselves are still present and tracked).

## [0.3.0] - 2026-07-12

Initial firmware and hardware design in the repo.

### Added
- `LICENSE` (MIT).
- `src/main.cpp` with the full dice-rolling logic: PRNG, button interrupt,
  sleep mode, dice face mask table, startup test, pin mapping.
- KiCad schematic and PCB project (originally named `digital-dice`).
- `platformio.ini` configured for `attiny13a` env, USBasp upload protocol.
- Extended `.gitignore` for KiCad local artifacts and PlatformIO `.pio/`.
- Internal documentation: `docs/kicad-schematic-checklist.md`,
  `docs/schematic-v1.md`.

## [0.2.0] - 2024-11-03

Documentation refresh.

### Changed
- README updated with project description, build instructions, and
  ATtiny13A-specific notes.

## [0.1.0] - 2023-11-24

Initial commit.

### Added
- Empty PlatformIO project skeleton (`platformio.ini`, `src/main.cpp`,
  `include/`, `lib/`, `test/`).
- `.gitignore` covering build outputs.
- `.vscode/extensions.json` recommending PlatformIO IDE.
- First README.

[Unreleased]: https://github.com/VorosEgyes/ArcDice/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/VorosEgyes/ArcDice/compare/380ab67...v1.0.0
[0.9.0]: https://github.com/VorosEgyes/ArcDice/compare/c320930...380ab67
[0.5.0]: https://github.com/VorosEgyes/ArcDice/compare/7bee596...afaace9
[0.4.0]: https://github.com/VorosEgyes/ArcDice/compare/b4b2c73...7bee596
[0.3.0]: https://github.com/VorosEgyes/ArcDice/compare/8829135...b4b2c73
[0.2.0]: https://github.com/VorosEgyes/ArcDice/compare/8829135...d1e2978
[0.1.0]: https://github.com/VorosEgyes/ArcDice/tree/c320930