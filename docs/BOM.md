# Bill of Materials (BOM)

This document lists every component needed to build one ArcDice unit.
It is derived from the KiCad project (`hardware/ArcDice.kicad_pro`,
`.kicad_sch`, `.kicad_pcb`) and the firmware pin mapping in
`src/main.cpp`. Footprints are taken straight from the KiCad library
references.

> **Sourcing note** — KiCad footprints identify the *physical package*
> (e.g. `LED_D5.0mm`, `R_Axial_DIN0207`). For exact distributor part numbers
> (Digi-Key, Mouser, LCSC), see the *Suggested supplier parts* column. Any
> equivalent part that matches footprint and electrical specs will work.

## Components

| Ref | Qty | Part (KiCad) | Description | Footprint / package | Suggested supplier part |
|----:|----:|--------------|-------------|---------------------|--------------------------|
| U1 | 1 | `ATtiny13A-P` | 8-bit AVR MCU, 1 KB flash, 64 B EEPROM, 64 B SRAM, internal 9.6 MHz oscillator | DIP-8 (through-hole) | Microchip ATtiny13A-PU (Digi-Key ATATTINY13A-PU-ND) |
| D1–D7 | 7 | `LED` (generic) | 5 mm through-hole LED — pick your colour (red, yellow, green, blue, white all fit). Forward voltage ~2.0 V (red) to ~3.2 V (blue/white) at 20 mA | `LED_THT:LED_D5.0mm` | Kingbright L-7113ID (red, THT, 5 mm) or any 5 mm LED of your choice |
| R1–R7 | 7 | Resistor 220 Ω, ¼ W | LED current-limiting. With 3.0 V Li-ion and a red LED (~2 V Vf), 220 Ω gives ~5 mA per LED — plenty for a battery-powered dice | `Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal` | Yageo MFR-25FBF52-220R (Digi-Key 220XBK-ND) |
| SW1 | 1 | `SW_Push` — roll button | 6 mm × 9.5 mm through-hole tactile push button, SPST-NO. Connected to PB4 (ATtiny13A RESET pin) to GND, internal pull-up enabled in firmware | `Button_Switch_THT:SW_PUSH_6mm_H9.5mm` | C&K PTS810SJM250SMTRLFS or any equivalent 6 mm THT tactile |
| SW2 | 1 | `Switch_Slide_SS12D06_Generic` | SPDT slide switch — most likely the power on/off switch between battery and TP4056 / load | `Button_Switch_THT:Switch_Slide_SS12D06_Generic` | SS12D06G3 or any equivalent SPDT slide switch, THT |
| SW3 | 1 | `SW_SPDT` | Second SPDT switch — check schematic for exact use (likely mode-select or reset disable) | (KiCad symbol `Switch:SW_SPDT`, footprint TBD) | Any SPDT THT slide/toggle switch |
| BT1 | 1 | `Battery_Cell` | Single 3.7 V Li-ion / LiPo cell (1S), nominal capacity 100–500 mAh typical for a dice. Use a protected cell (with BMS) and add a 1N4148 flyback diode if driving the LEDs without other regulation | Through-hole battery holder footprint (KiCad `Device:Battery_Cell`) | 1S Li-ion/LiPo |
| U2 | 1 | `TP4056_Module` | TP4056 Li-ion/LiPo linear charging module, micro-USB or USB-C input depending on the module variant. Charges the cell from any 5 V source; module typically includes a DW01 + FS8205 protection IC. Note: the **module** is mounted, not the bare TP4056 IC | Through-hole 4-pin module (OUT+/OUT−/IN+/IN−) | TP4056 micro-USB module (LCSC C103626) or USB-C variant (LCSC C2932096) |
| C1, C2 | 2 | Decoupling capacitors | Polarized + unpolarized decoupling. Typical: 100 nF ceramic (unpolarized) on ATtiny AVCC, and 10 µF electrolytic (polarized) across battery / TP4056 output for smooth LED current | THT, 5 mm pitch (KiCad `Device:C` and `Device:CP`) | Murata RDER71H104K (100 nF) + Nichicon UPW1A100MDD (10 µF) |
| H1–H4 | 4 | M2 mounting holes, 2.2 mm drill | Mechanical mounting only — use with M2 × 4 (or longer) pan-head screws and (optional) 4 mm hex spacers | `MountingHole:MountingHole_2.2mm_M2` | M2 × 4 pan-head screw + M2 hex spacer, 4 each |

## Component count summary

- **Active:** 1 MCU + 1 charger module = **2**
- **Passive:** 7 resistors + 2 capacitors = **9**
- **LEDs:** 7
- **Switches:** 3 (1 push button + 2 SPDT slide switches)
- **Battery:** 1 cell (1S Li-ion/LiPo, user-supplied, not part of a bare kit)
- **Mechanical:** 4 M2 mounting holes

## What is NOT on this BOM

- **USBasp programmer** — required to flash the ATtiny13A initially, but not part of the assembled board.
- **High-voltage (HV) programmer** — required to set the `RSTDISBL` fuse (which lets PB4 be used as a button input), and **required** to ever reflash the chip after `RSTDISBL` is set. A DIY "HV rescue shield" with an Arduino Uno + 12 V source works; or use an Atmel-ICE in HV mode.
- **Battery** — listed above for reference but typically user-supplied (a protected 1S cell with leads).
- **Enclosure / 3D-printed case** — not in the current hardware design.

## Notes for sourcing

1. **LED colour** — pick whatever you prefer; the firmware doesn't depend on it. If you mix colours, double-check the forward voltage: a 220 Ω resistor is fine for red/yellow/green (~2 V Vf). For blue/white (~3 V Vf) you'll want closer to 100 Ω to keep the LED brightness matched at ~5–10 mA.
2. **Resistor power rating** — 220 Ω × 5 mA = ~5.5 mW per resistor, so ¼ W is enormous overkill; even 1/8 W works.
3. **TP4056 module vs. bare IC** — the BOM uses the *module*, not the bare TP4056 IC, because the module integrates the DW01 + FS8205 protection and the micro-USB connector. If you want to redesign around the bare IC (smaller PCB, no on-board USB), that is a v2 hardware decision, not a BOM change.
4. **Battery polarity** — make sure your 1S cell has a protection circuit (BMS). The TP4056 module by itself does not include over-discharge protection.

## Sourcing shortcuts

| Supplier | Region | Best for |
|----------|--------|----------|
| LCSC | China / EU warehouse | TP4056 module, basic passives, LEDs |
| Digi-Key | Worldwide | ATtiny13A-PU (genuine Microchip), quality passives |
| Mouser | Worldwide | Same as Digi-Key, often better for European orders |
| TME | EU (Hungary-friendly) | Through-hole resistors, capacitors, LEDs, switches |
| Reichelt | DE / EU | German warehouse, good for DIN0207 resistors |
| AliExpress | China | Cheapest TP4056 modules and LEDs, expect 2–4 week shipping |