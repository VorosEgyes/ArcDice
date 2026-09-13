---
layout: default
title: ATtiny13 Digital Dice
---

<a id="hero"></a>

<div class="hero">
  <div class="hero-text">
    <h1>ArcDice</h1>
    <p class="tagline">An open-source digital dice built from an ATtiny13A — 1 KB of flash, 7 LEDs, one button, infinite rolls.</p>
    <div class="cta-row">
      <a class="cta-button" href="{{ '/#download' | relative_url }}">Download v1.1.0</a>
      <a class="cta-button secondary" href="{{ site.github_url }}">View on GitHub ↗</a>
      <a class="cta-button secondary" href="{{ '/#demo' | relative_url }}">Watch demo</a>
    </div>
  </div>
  <div class="hero-image">
    <img src="{{ '/images/assembled-hero.jpg' | relative_url }}" alt="ArcDice assembled prototype with all 7 LEDs lit showing a 6">
  </div>
</div>

## What is it?

ArcDice is a complete DIY electronics project: a pocket-sized digital dice you build yourself. Press the button, watch the 7-LED display animate, and a random number from 1 to 6 appears.

It's built around an **ATtiny13A** — one of the smallest AVR microcontrollers Atmel makes, with just **1 KB of flash** and **64 bytes of RAM**. Driving 7 LEDs from only 4 I/O pins uses a clever charlieplexing scheme; the firmware fits in 598 bytes (58.4% of available flash).

The hardware is open (KiCad schematics + PCB layout, MIT licensed), the firmware is open (C++, 39 source files, host-native unit tests), and the BOM has 10 component lines / 26 parts — most of which you probably already have in your parts bin.

<a id="hardware"></a>

## Hardware

- **MCU**: ATtiny13A in DIP-8 (8-bit AVR, 1 KB flash, 64 B RAM, internal 9.6 MHz oscillator)
- **Display**: 7 red LEDs arranged in the classic dice-6 pattern, charlieplexed on PB0–PB3
- **Input**: Single tactile button (wake from sleep on press)
- **Power**: TP4056 Li-ion/LiPo charger module (USB-C or micro-USB variant) with built-in DW01 + FS8205 protection IC
- **Cell**: 1S Li-ion/LiPo (protected, user-supplied, ~600 mAh gives weeks of standby)

### PCB

<div class="gallery-grid">
  <div class="gallery-item">
    <img src="{{ '/images/pcb-front-bare.jpg' | relative_url }}" alt="PCB front side, unpopulated">
    <p class="gallery-caption">PCB front side (bare)</p>
  </div>
  <div class="gallery-item">
    <img src="{{ '/images/pcb-back-bare.jpg' | relative_url }}" alt="PCB back side, unpopulated">
    <p class="gallery-caption">PCB back side (bare)</p>
  </div>
</div>

### Specifications

<table class="spec-table">
  <tr><th>Hardware revision</th><td>v2.1 (KiCad)</td></tr>
  <tr><th>PCB size</th><td>~30 × 30 mm</td></tr>
  <tr><th>Layers</th><td>2 (top + bottom)</td></tr>
  <tr><th>BOM</th><td>10 component lines, 26 parts total</td></tr>
  <tr><th>Power consumption (idle)</th><td>< 10 µA (sleep mode)</td></tr>
  <tr><th>Power consumption (roll)</th><td>~15 mA peak (all 7 LEDs)</td></tr>
  <tr><th>Build time</th><td>~30 minutes (THT parts only)</td></tr>
  <tr><th>License</th><td>MIT</td></tr>
</table>

<a id="demo"></a>

## Demo

<div class="demo-gif">
  <img src="{{ '/arcdice-demo.gif' | relative_url }}" alt="ArcDice demo: button press animates LEDs to a 3">
  <p><em>Press the button — the 7 LEDs animate for ~1.5 seconds, then settle on a random number from 1 to 6.</em></p>
</div>

The dice face logic is unit-tested against the canonical patterns in `include/dice.h`, with a host-native test runner that runs on any laptop in under a second (`pio test -e native`).

<a id="gallery"></a>

## Gallery

<div class="gallery-grid">
  <div class="gallery-item">
    <img src="{{ '/images/assembled-hero.jpg' | relative_url }}" alt="Assembled ArcDice showing a 6">
    <p class="gallery-caption">All 7 LEDs lit — a 6</p>
  </div>
  <div class="gallery-item">
    <img src="{{ '/images/assembled-side-angle.jpg' | relative_url }}" alt="Side angle showing PCB thickness">
    <p class="gallery-caption">Side view — battery + TP4056 visible</p>
  </div>
  <div class="gallery-item">
    <img src="{{ '/images/hero.jpg' | relative_url }}" alt="Hero shot, top-down">
    <p class="gallery-caption">Top-down view</p>
  </div>
</div>

## Build it yourself

The project ships with everything you need:

- **Pre-compiled firmware** (`ArcDice_v1.1.0_firmware.hex`) — flash with a $5 USBasp and `avrdude` in 30 seconds
- **Gerber package** (`ArcDice_v2.1_gerber.zip`) — upload the inner `Archive.zip` to PCBWay or JLCPCB; ~$5 for 5 boards
- **BOM CSV** (`ArcDice_v2.1_bom.csv`) — 10 lines with KiCad footprints and suggested supplier parts (Digi-Key / LCSC / Mouser)
- **Assembly guide** — step-by-step build instructions with a troubleshooting table

Total cost of parts: roughly **$8–12** if you're sourcing everything new, less if you have a parts bin.

```bash
# Flash the firmware
avrdude -c usbasp -p attiny13 -U flash:w:ArcDice_v1.1.0_firmware.hex:i
```

The full build takes about 30 minutes if you've soldered through-hole before.

<a id="download"></a>

## Download

Latest release: **[v1.1.0 — Hardware v2.1]({{ site.github_url }}/releases/tag/v1.1.0)** (2026-09-11)

| Asset | Size | Description |
|-------|------:|-------------|
| [ArcDice_v1.1.0_firmware.hex]({{ site.github_url }}/releases/download/v1.1.0/ArcDice_v1.1.0_firmware.hex) | 1.7 KB | Pre-compiled ATtiny13A firmware |
| [ArcDice_v2.1_gerber.zip]({{ site.github_url }}/releases/download/v1.1.0/ArcDice_v2.1_gerber.zip) | 208 KB | PCBWay-ready Gerber package |
| [ArcDice_v2.1_bom.csv]({{ site.github_url }}/releases/download/v1.1.0/ArcDice_v2.1_bom.csv) | 2.7 KB | Bill of Materials (10 lines, 26 parts) |
| [ArcDice_v2.1_assembly_guide.md]({{ site.github_url }}/releases/download/v1.1.0/ArcDice_v2.1_assembly_guide.md) | 4.8 KB | 6-step build instructions |

**SHA256** (firmware):
```
3bb64ebeb0fcdb910fa978c3b29c1daba74df96bf88c5c467d6e60e317c714ee
```

Always verify before flashing.

## License

MIT — see [LICENSE]({{ site.github_url }}/blob/main/LICENSE). Schematics, PCB layout, and firmware are all released under the same terms. Feel free to fork, modify, and build your own version.
