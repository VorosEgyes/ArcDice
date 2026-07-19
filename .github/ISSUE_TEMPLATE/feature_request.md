---
name: Feature request
about: Suggest an improvement — a new feature, a refactor, or a behaviour change.
title: "[FEAT] "
labels: ["feature"]
assignees: []
---

## Problem

<!-- What limitation or missing capability prompted this request?
     Describe the use case, not just the solution. -->

## Proposed solution

<!-- Sketch the API, schematic change, or user-facing behaviour.
     A picture, a KiCad snippet, or pseudocode is welcome. -->

## Alternatives considered

<!-- Other approaches you weighed and why you didn't pick them.
     Not strictly required but it speeds up review. -->

## Affected parts

<!-- Tick all that apply. -->
- [ ] Firmware (`src/main.cpp`, `include/dice.h`)
- [ ] Hardware schematic (`hardware/ArcDice.kicad_sch`)
- [ ] Hardware PCB (`hardware/ArcDice.kicad_pcb`)
- [ ] Bill of materials (`docs/BOM.md`)
- [ ] Documentation (`README.md`, `CHANGELOG.md`)
- [ ] CI / tests (`.github/workflows/`, `test/`)
- [ ] Just an idea, no specific part yet

## Will you send a PR?

<!-- If yes, mention any specific design choices you want the
     maintainer to weigh in on *before* you start coding. If no,
     that's fine — feature requests without a PR are still useful. -->

## Checklist

- [ ] I searched existing issues and discussions to avoid duplicates.
- [ ] The change fits within the ATtiny13A's 1 KB flash budget
      (for firmware changes).