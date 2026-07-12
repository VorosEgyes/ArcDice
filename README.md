# ATtiny13 Digital Dice

## Overview
This repository contains the Arduino code for a digital dice project using an ATtiny13 microcontroller. The digital dice simulates a rolling dice effect on a 7-LED dice face that is driven from 4 MCU output pins, then enters a power-saving mode when not in use.

## Features
- Dice rolling effect using LEDs.
- 7-LED dice layout driven from 4 output pins.
- Power efficiency with ATtiny13 sleep mode.
- Button debouncing for reliable operation.
- Randomized dice results.

## Hardware Requirements
- ATtiny13 microcontroller
- LEDs (x7), arranged as a dice face and driven from 4 outputs
- Tactile pushbutton
- 100 ohm Resistors (x7) for LEDs
- 10K ohm Resistor for pullup reset
- 2xAAA Power supply (2.4-3V)
- (Optional) USBASP Programmer for ATtiny13
- (Optional) a 100nF ceramic condensator between VCC and GND

## LED Layout
The 7 LEDs form the visible dice face in this arrangement:

```text
LED   .   LED
LED  LED  LED
LED   .   LED
```

The display is controlled from 4 ATtiny13 output pins rather than 7 dedicated GPIO lines.

## Wiring

The LED section uses 4 ATtiny13 output pins for the display and one additional pin for the pushbutton input.

In the firmware, those 4 display control lines are treated as 4 visible LED groups:

- `SEGMENT_CENTER`: the center LED
- `SEGMENT_DIAGONAL_A`: the top-left and bottom-right LEDs
- `SEGMENT_DIAGONAL_B`: the top-right and bottom-left LEDs
- `SEGMENT_MIDDLE_ROW`: the left-middle and right-middle LEDs

That grouping is what allows the classic 7-point dice face to be shown with only 4 output-controlled display lines.

## Schematic Baseline (KiCad v1)

This project baseline now targets:

- 1-cell Li-ion/LiPo battery (3.7V nominal, 4.2V full)
- TP4056 charging module, protected type (B+/B-/OUT+/OUT-)
- No ISP header on-board
- ON/OFF main power switch between charger OUT+ and system VCC

Power chain:

- USB 5V -> TP4056 IN+/IN-
- Battery -> TP4056 B+/B-
- TP4056 OUT+ -> SW1 (main switch) -> VCC_SYS
- TP4056 OUT- -> GND

MCU and I/O:

- PB0: `SEGMENT_CENTER`
- PB1: `SEGMENT_DIAGONAL_A`
- PB2: `SEGMENT_DIAGONAL_B`
- PB3: `SEGMENT_MIDDLE_ROW`
- PB4: roll button input (button to GND, internal pull-up enabled in firmware)

LED mapping for the 7 visible dice points:

- `SEGMENT_CENTER`: center LED
- `SEGMENT_DIAGONAL_A`: top-left and bottom-right LEDs
- `SEGMENT_DIAGONAL_B`: top-right and bottom-left LEDs
- `SEGMENT_MIDDLE_ROW`: left-middle and right-middle LEDs

Recommended LED resistors (updated):

- Use 220 ohm per LED (7 total) as default for 3.7V battery operation.
- Reason: at 4.2V full charge and about 2.0V LED forward voltage, current is about (4.2V - 2.0V) / 220R = 10 mA per LED.
- This is safer for ATtiny13 pin and total current limits than 100 ohm.

For full KiCad net naming and BOM starter details, see `docs/schematic-v1.md`.
For a step-by-step drawing checklist in KiCad, see `docs/kicad-schematic-checklist.md`.

<img width="281" alt="Screenshot 2023-11-24 at 22 39 49" src="https://github.com/VorosEgyes/digital-dice/assets/69361851/ae5dc22b-e800-4465-bd64-4f9b8bfa3590">

## Software Setup
1. Use platformio
2. Connect the ATtiny13 to your computer via an USBASP Programmer.
3. Upload the code to the ATtiny13.

## Usage
Press the button to "roll" the dice. The LEDs will display a random number from 1 to 6. After displaying the result, the device will enter sleep mode to conserve power.

## Contributing
Contributions to this project are welcome! Please fork the repository and submit a pull request with your improvements.

## License
This project is licensed under the MIT License

## Acknowledgements
- Thanks to the Arduino and AVR communities for support and resources.
- Project inspired by http://www.avr-asm-tutorial.net/avr_en/apps/dice/dice_tn13/dice.html

