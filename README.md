# ArcDice

ArcDice is a compact ATtiny13-based digital dice project with a 7-LED face, low-power sleep behavior, and a matching KiCad hardware design.

Keywords: digital dice, ATtiny13 dice, LED dice, DIY electronic dice.

## What It Does

- Rolls a random value from 1 to 6 with a short LED animation.
- Drives a 7-point dice face from only 4 MCU output lines.
- Uses button wake-up and sleep mode to reduce power draw.

## Firmware

- Source: `src/main.cpp`
- PlatformIO environment: `attiny13a`
- Upload protocol: `usbasp`

### Pin Mapping

- PB0 -> center LED segment
- PB1 -> diagonal A (top-left + bottom-right)
- PB2 -> diagonal B (top-right + bottom-left)
- PB3 -> middle row (left-middle + right-middle)
- PB4 -> roll button (to GND, internal pull-up)

## Hardware

- KiCad project files:
	- `hardware/ArcDice.kicad_pro`
	- `hardware/ArcDice.kicad_sch`
	- `hardware/ArcDice.kicad_pcb`
- Power target: 1S Li-ion/LiPo with TP4056 protected module
- Recommended LED resistors: 220 ohm (one per LED)

## Images

### 3D Top

![ArcDice 3D top](docs/arcdice-3d-top.png)

### 3D Bottom

![ArcDice 3D bottom](docs/arcdice-3d-bottom.png)

### Schematic v1

![ArcDice schematic v1](docs/arcdice-schematic-v1.png)

## Build And Upload

1. Install PlatformIO.
2. Connect an ATtiny13 programmer (USBasp).
3. Build and upload:

```bash
pio run -e attiny13a -t upload
```

## License

MIT. See `LICENSE`.

