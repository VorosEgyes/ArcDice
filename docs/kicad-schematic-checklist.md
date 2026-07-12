# KiCad Schematic Build Checklist (v1)

This checklist is for building `hardware/digital-dice.kicad_sch` from scratch using the agreed constraints.

## 1) Project Setup

- Open `hardware/digital-dice.kicad_pro`.
- In schematic editor, set sheet title fields.
- Confirm all symbols use stable libraries (avoid temporary local paths when possible).

## 2) Place Core Blocks

Place these symbols first:

- U1: ATtiny13A (package choice must match intended PCB assembly)
- U2: TP4056 module interface (either module symbol or connector-style symbol)
- BT1: 1S battery connector
- SW1: ON/OFF power switch
- SW2: roll pushbutton
- D1..D7: dice LEDs
- R1..R7: 220 ohm LED resistors
- R8: 10k reset pull-up
- C1: 100 nF decoupling
- C2: 10 uF optional bulk

## 3) Define Power Nets

Create and label these nets exactly:

- USB_5V
- BAT_PLUS
- BAT_MINUS
- SYS_VCC
- GND

Wire power path:

- TP4056 IN+ -> USB_5V
- TP4056 IN- -> GND
- TP4056 B+ -> BAT_PLUS
- TP4056 B- -> BAT_MINUS
- TP4056 OUT+ -> SW1 -> SYS_VCC
- TP4056 OUT- -> GND

## 4) ATtiny13 Wiring

Wire U1:

- VCC -> SYS_VCC
- GND -> GND
- PB0 -> SEGMENT_CENTER
- PB1 -> SEGMENT_DIAGONAL_A
- PB2 -> SEGMENT_DIAGONAL_B
- PB3 -> SEGMENT_MIDDLE_ROW
- PB4 -> BTN_ROLL
- RESET/PB5 -> RESET_N

Support parts:

- R8: 10k between RESET_N and SYS_VCC
- C1: 100 nF between SYS_VCC and GND, physically near U1
- C2: 10 uF between SYS_VCC and GND (optional)

### ATtiny13 Pin Number Map (for wiring without ambiguity)

Use this pin map when assigning nets in KiCad.

#### DIP-8 (ATtiny13A-PU)

- Pin 1: RESET/PB5 -> RESET_N
- Pin 2: PB3 -> SEGMENT_MIDDLE_ROW
- Pin 3: PB4 -> BTN_ROLL
- Pin 4: GND -> GND
- Pin 5: PB0 -> SEGMENT_CENTER
- Pin 6: PB1 -> SEGMENT_DIAGONAL_A
- Pin 7: PB2 -> SEGMENT_DIAGONAL_B
- Pin 8: VCC -> SYS_VCC

#### SOIC-8 (ATtiny13A-SSU)

- Pin 1: RESET/PB5 -> RESET_N
- Pin 2: PB3 -> SEGMENT_MIDDLE_ROW
- Pin 3: PB4 -> BTN_ROLL
- Pin 4: GND -> GND
- Pin 5: PB0 -> SEGMENT_CENTER
- Pin 6: PB1 -> SEGMENT_DIAGONAL_A
- Pin 7: PB2 -> SEGMENT_DIAGONAL_B
- Pin 8: VCC -> SYS_VCC

Note: logical pin mapping is identical between DIP-8 and SOIC-8; only the footprint package changes.

## 5) Button Wiring

- SW2 between BTN_ROLL and GND
- No external pull-up needed (firmware uses internal pull-up)

## 6) LED Group Wiring (7 visible LEDs)

Visible dice points:

- Top-left, Top-right
- Mid-left, Mid-center, Mid-right
- Bottom-left, Bottom-right

Group assignment:

- SEGMENT_CENTER -> Mid-center LED only
- SEGMENT_DIAGONAL_A -> Top-left and Bottom-right LEDs
- SEGMENT_DIAGONAL_B -> Top-right and Bottom-left LEDs
- SEGMENT_MIDDLE_ROW -> Mid-left and Mid-right LEDs

Resistors:

- One resistor per LED (R1..R7), 220 ohm nominal
- Keep resistor designators consistent with LED position naming

## 7) Naming Convention (recommended)

LED names to avoid ambiguity:

- D1 TOP_LEFT
- D2 TOP_RIGHT
- D3 MID_LEFT
- D4 MID_CENTER
- D5 MID_RIGHT
- D6 BOT_LEFT
- D7 BOT_RIGHT

Then map nets:

- D4 -> SEGMENT_CENTER
- D1 + D7 -> SEGMENT_DIAGONAL_A
- D2 + D6 -> SEGMENT_DIAGONAL_B
- D3 + D5 -> SEGMENT_MIDDLE_ROW

## 8) ERC / Sanity Checks

Run ERC and verify:

- No unconnected power pins
- No floating RESET pin
- All LED resistor paths are in series, not bypassed
- SW1 actually disconnects SYS_VCC from charger output
- BTN_ROLL is only pulled by MCU internal pull-up and switch to GND

## 9) Firmware Consistency Check

Ensure schematic nets match firmware mapping in `src/main.cpp`:

- PB0 = SEGMENT_CENTER
- PB1 = SEGMENT_DIAGONAL_A
- PB2 = SEGMENT_DIAGONAL_B
- PB3 = SEGMENT_MIDDLE_ROW
- PB4 = BUTTON_PIN

## 10) Pre-PCB Checklist

- Annotate and re-annotate once
- Assign footprints to all symbols
- Generate BOM preview
- Generate netlist/update PCB from schematic
- Save a PDF export as design freeze draft
