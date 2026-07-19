# Contributing to ArcDice

Thank you for your interest in improving ArcDice! This document explains the
project's workflow and conventions so your changes can be reviewed and merged
quickly.

## Code of Conduct

By participating in this project you agree to abide by the
[Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). Be respectful.
Assume good faith. Disagree on substance, not on personality.

## Project layout

```
ArcDice/
├── src/                   — ATtiny13A firmware (C++ / Arduino framework)
├── include/dice.h         — pure-logic face-mask table (host-testable)
├── test/                  — host-native Unity tests (run by CI)
├── platformio.ini         — PlatformIO environments
├── hardware/              — KiCad project + Gerber exports
│   ├── ArcDice.kicad_pro
│   ├── ArcDice.kicad_sch
│   ├── ArcDice.kicad_pcb
│   └── Gerber/V2/         — PCBWay-ready archive
├── docs/BOM.md            — bill of materials + sourcing
├── CHANGELOG.md           — release history (Keep-a-Changelog format)
├── README.md              — user-facing documentation
└── .github/
    ├── workflows/         — CI workflow files
    ├── ISSUE_TEMPLATE/    — bug report / feature request templates
    └── PULL_REQUEST_TEMPLATE.md
```

## Workflow

1. **Open an issue first** for anything beyond a small typo or one-line fix.
   The issue describes the *what* and *why*. The PR implements it.
2. **Fork the repository** and create a branch off `main`. Branch names:
   - `feat/<short-slug>` for new features
   - `fix/<short-slug>` for bug fixes
   - `docs/<short-slug>` for documentation-only changes
   - `hw/<short-slug>` for KiCad schematic / PCB changes
3. **Make your change.** Keep it focused — one logical change per PR.
4. **Run the CI locally before pushing** if you have PlatformIO installed:
   ```bash
   pio test -e native      # host unit tests (~20 s)
   pio run -e attiny13a    # AVR build + flash size check (~10 s with cache)
   ```
5. **Push** and **open a pull request** against `main`. Fill out the PR
   template. Link the issue it addresses (`Fixes #42`).
6. **Wait for CI.** The PR cannot be merged until both the
   `Host unit tests (native)` and `Build attiny13a firmware` jobs are green.
7. **Review.** The maintainer reviews the change. Expect questions, expect
   to iterate. A PR is not "done" until it is merged.

## Coding conventions

### C++ (firmware)

- **C++17 or later** is fine; the `native` test env enforces
  `-Wall -Wextra -Wpedantic`. Match the existing style — tabs for
  indentation, ~100-column soft wrap.
- **No magic numbers.** Pin numbers go in `include/dice.h`. Timing
  constants (`delay(110)`, `delay(200)`, `delay(25)`) should be
  `#define`-ed with a comment explaining the value choice. Sleep-related
  timing especially: keep the datasheet reference inline.
- **Hardware-specific code stays in `src/main.cpp`.** Anything that can
  live in `include/dice.h` (face-mask table, PRNG parameters, segment
  layout constants) should live in `include/dice.h` so the host test
  runner can validate it.
- **ISR handlers stay short.** Defer work to `loop()`. Touch only
  `volatile` state from inside the ISR.
- **Comments explain *why*, not *what*.** A line that needs a "what"
  comment is a line that probably needs a better name.

### KiCad (hardware)

- The project file is **KiCad 8** or later. Don't downgrade.
- Save backups via KiCad's built-in snapshot mechanism, *not* in the
  `hardware/ArcDice-backups/` folder. The `.gitignore` already excludes
  that folder so accidental backups don't leak.
- After a hardware change, **regenerate Gerber output** under
  `hardware/Gerber/V<N>/` and update `hardware/Gerber/V<N>/README.md`
  with the revision description.
- Tag every KiCad revision: `git tag hw-v1.0`, `hw-v1.1`, etc. The tag
  makes it possible to reproduce an old board revision long after the
  working tree has moved on.
- Run **DRC** and **ERC** before committing. Both should be clean.

### Commit messages

- Use the **Conventional Commits** style loosely:
  - `feat: add low-battery LED indicator`
  - `fix: correct PRNG polynomial to avoid 8-cycle degeneracy`
  - `docs: clarify RSTDISBL trade-off in README`
  - `hw: swap PB4 button footprint for a lower-profile SW_PUSH`
  - `ci: cache PlatformIO packages by src/ hash`
  - `test: add edge cases for out-of-range face masks`
  - `chore: bump .gitignore for KiCad 8`
- Subject line **50–72 chars**, imperative mood, no trailing period.
- Body is a short paragraph (or bullet list) explaining what changed and
  why. Wrap at 72 cols.
- Footer: `Fixes #NN` or `Refs #NN` when the commit addresses an issue.

### Documentation

- **English only.** Translations live in a separate repo or are added
  by a follow-up PR once the English version has been reviewed.
- README sections follow the existing structure (Description → Firmware
  → Hardware → Images → Build → License).
- New components or rev changes go in `CHANGELOG.md` *and*
  `docs/BOM.md` (and `hardware/Gerber/V<N>/README.md` for HW).

## Testing

The CI pipeline runs two jobs on every PR:

1. **`Host unit tests (native)`** — `pio test -e native`, runs in ~20 s
   on a GitHub-hosted runner. Pure C++ tests against `include/dice.h`.
2. **`Build attiny13a firmware`** — `pio run -e attiny13a`, plus a
   flash-budget check that fails if `.text` exceeds 1024 bytes.

If you add a feature that touches the face-mask table or PRNG
parameters, **add a test** to `test/test_dice_masks/test_dice_masks.cpp`
or create a sibling file under `test/`.

If you change `src/main.cpp` but `include/dice.h` is unaffected, the
host tests will still pass — that's by design. The CI also catches AVR
build regressions via the second job.

## Issue labels (proposed)

- `bug` — confirmed defect
- `feature` — new capability request
- `docs` — documentation-only change
- `hw` — KiCad schematic / PCB
- `firmware` — `src/main.cpp` or `include/dice.h`
- `good first issue` — small, well-scoped, suitable for newcomers
- `help wanted` — the maintainer would like a PR but doesn't have
  bandwidth to do it themselves

## Release flow

The maintainer handles releases. You don't need to bump versions or
update `CHANGELOG.md` in your PR — that's done at release-prep time so
the version number and date are accurate.

## Questions?

Open an issue. If it's about your specific change, leave it as a
comment on the PR. Don't email the maintainer directly unless the
issue is sensitive (security disclosure, CoC violation).