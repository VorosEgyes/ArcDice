## Summary

<!-- One or two sentences describing what this PR does. -->

## Linked issue

<!-- "Fixes #NN" or "Refs #NN". If the PR isn't tied to an issue,
     say so and explain why. -->

## Type of change

<!-- Tick one. Delete the others. -->
- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that would cause existing behaviour to change)
- [ ] Documentation / CI / tooling only (no production code change)

## Affected parts

<!-- Tick all that apply. -->
- [ ] Firmware (`src/main.cpp`, `include/dice.h`)
- [ ] Hardware (`hardware/`)
- [ ] Tests (`test/`)
- [ ] CI (`.github/workflows/`)
- [ ] Documentation (`README.md`, `CHANGELOG.md`, `docs/`, `CONTRIBUTING.md`)
- [ ] Bill of materials (`docs/BOM.md`)

## Test plan

<!-- How did you verify the change?
     For firmware: did you run `pio test -e native`? `pio run -e attiny13a`?
     For hardware: did you run KiCad DRC and ERC? Did you order a panel?
     For docs: did you preview the Markdown? -->

- [ ] Host unit tests pass (`pio test -e native`)
- [ ] AVR build passes (`pio run -e attiny13a`)
- [ ] Firmware `.text` section ≤ 1024 bytes (ATtiny13A flash budget)
- [ ] Manual hardware test (only for HW changes)
- [ ] Documentation previewed / proofread

## Checklist

- [ ] I read [CONTRIBUTING.md](../CONTRIBUTING.md).
- [ ] My commit messages follow the project's conventions (conventional
      commits, ≤ 72-char subject, body explains *why*).
- [ ] I added or updated [CHANGELOG.md](../CHANGELOG.md) under
      `[Unreleased]` if this is a user-visible change.
      *(The maintainer may collapse it into a release at merge time.)*
- [ ] I added or updated tests for any changed behaviour.
- [ ] I did NOT commit any secrets, build artifacts (`.pio/`),
      KiCad backups (`hardware/*-backups/`), or local IDE state.