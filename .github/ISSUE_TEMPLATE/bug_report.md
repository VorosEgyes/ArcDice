---
name: Bug report
about: Something is broken — a feature that doesn't work, a regression, or unexpected behaviour.
title: "[BUG] "
labels: ["bug"]
assignees: []
---

## Summary

<!-- One sentence describing what's broken. -->

## Steps to reproduce

1.
2.
3.

## Expected behaviour

<!-- What you expected to happen. -->

## Actual behaviour

<!-- What actually happened. Include the LED pattern, error messages,
     compile output, or anything else that helps explain. -->

## Environment

- **Board revision**: KiCad `V1` / `V2` / custom — see `hardware/Gerber/V<N>/README.md`
- **MCU firmware version**: (e.g. `v1.0.0` from CHANGELOG, or commit SHA)
- **Fuses**: (e.g. `RSTDISBL` programmed or stock)
- **Battery**: (e.g. protected 1S Li-ion, ~3.7 V nominal, capacity)
- **LED colour** (if relevant to the bug): (e.g. red 5 mm THT, Vf ≈ 2.0 V)

## Logs / screenshots

<!-- Paste `pio run -e attiny13a` output, terminal session, or photos
     of the board. Wrap logs in triple backticks for readability. -->

```text
paste here
```

## Possible cause

<!-- Optional. If you have a hunch about what's going wrong, share it.
     Even a wrong guess is useful — it gives the maintainer a starting point. -->

## Checklist

- [ ] I searched existing issues to avoid duplicates.
- [ ] I checked the [CHANGELOG.md](../CHANGELOG.md) — this is not a known
      regression from a recent release.
- [ ] I have built the firmware myself with the latest `main` and the
      bug reproduces there.