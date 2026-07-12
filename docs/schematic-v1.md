# Digital Dice Schematic v1 (KiCad Baseline)

## Scope

This file defines the initial schematic target that will be implemented in KiCad.

Chosen constraints:

- 1S Li-ion/LiPo battery: 3.7V nominal, 4.2V full
- TP4056 module: protected type (B+/B-/OUT+/OUT-)
- No ISP header on board
- Main ON/OFF switch included
- No load-sharing power-path circuit (simple topology)

## Power Topology

- USB 5V input connects to TP4056 IN+ and IN-
- Battery cell connects to TP4056 B+ and B-
- TP4056 OUT+ feeds SW1, then VCC_SYS
- TP4056 OUT- is system GND

System rail:

- VCC_SYS = battery voltage after protection and switch
- Expected range: about 3.0V to 4.2V

## ATtiny13 Connections

- VCC -> VCC_SYS
- GND -> GND
- PB0 -> SEGMENT_CENTER net
- PB1 -> SEGMENT_DIAGONAL_A net
- PB2 -> SEGMENT_DIAGONAL_B net
- PB3 -> SEGMENT_MIDDLE_ROW net
- PB4 -> BTN_ROLL net
- RESET/PB5 -> keep reset function, add 10k pull-up to VCC_SYS and optional test pad

Decoupling:

- 100 nF ceramic capacitor directly between VCC and GND at ATtiny13 pins
- Optional 10 uF bulk capacitor on VCC_SYS near MCU and LED network

## Button Circuit

- One tactile switch between PB4 (BTN_ROLL) and GND
- Internal pull-up in firmware is used (no external pull-up required)

## LED Network

Visible dice layout:

- Top row: left, right
- Middle row: left, center, right
- Bottom row: left, right

Group mapping used by firmware:

- SEGMENT_CENTER -> center LED only
- SEGMENT_DIAGONAL_A -> top-left and bottom-right LEDs
- SEGMENT_DIAGONAL_B -> top-right and bottom-left LEDs
- SEGMENT_MIDDLE_ROW -> middle-left and middle-right LEDs

Series resistors:

- Use one resistor per LED (7 total)
- Recommended value: 220 ohm

Current check at full battery:

- Assume Vf(LED) about 2.0V
- I = (4.2V - 2.0V) / 220R about 10 mA per active LED

## TP4056 Notes

- Use module variant with battery protection board (DW01A + 8205A style)
- Charge current target: 500 mA (typical PROG about 2k on module design)
- Verify module documentation; some boards have fixed resistor already populated

## Initial Net Labels (for KiCad)

Power nets:

- USB_5V
- CHG_IN_GND
- BAT_PLUS
- BAT_MINUS
- SYS_VCC
- GND

MCU/display nets:

- SEGMENT_CENTER
- SEGMENT_DIAGONAL_A
- SEGMENT_DIAGONAL_B
- SEGMENT_MIDDLE_ROW
- BTN_ROLL
- RESET_N

## BOM Starter

- U1: ATtiny13A (package as selected for PCB)
- U2: TP4056 protected charging module footprint or connector interface
- BT1: 1S Li-ion/LiPo cell connector
- SW1: main ON/OFF switch
- SW2: roll button
- D1..D7: LEDs
- R1..R7: 220 ohm (LED series)
- R8: 10k (RESET pull-up)
- C1: 100 nF (MCU decoupling)
- C2: 10 uF (optional bulk)

## Firmware Alignment

Firmware segment pin mapping is currently:

- PB0 = SEGMENT_CENTER
- PB1 = SEGMENT_DIAGONAL_A
- PB2 = SEGMENT_DIAGONAL_B
- PB3 = SEGMENT_MIDDLE_ROW
- PB4 = BUTTON_PIN

If schematic net names or pin assignment changes, update src/main.cpp accordingly.
