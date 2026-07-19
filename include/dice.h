// dice.h — Pure-logic header for the dice face mapping.
//
// This file is intentionally free of Arduino-specific includes so it can
// be compiled both by the ATtiny13A firmware and by the host-native test
// runner (PlatformIO `pio test -e native`).
//
// The hardware layout (PB0..PB3 as display segments, PB4 as the button)
// is fixed in stone at the schematic level — see docs/BOM.md and the
// main README. Changing these values here without changing the firmware
// pin mapping AND the KiCad schematic will produce a board that lights
// up the wrong segments.

#ifndef ARCDICE_DICE_H_
#define ARCDICE_DICE_H_

#include <stdint.h>

// Bit positions on ATtiny13A PORTB. Keep in sync with src/main.cpp.
#ifndef DICE_SEGMENT_CENTER
#define DICE_SEGMENT_CENTER     0   // PB0 — center pip (CC)
#endif
#ifndef DICE_SEGMENT_DIAGONAL_A
#define DICE_SEGMENT_DIAGONAL_A 1   // PB1 — TL + BR diagonal
#endif
#ifndef DICE_SEGMENT_DIAGONAL_B
#define DICE_SEGMENT_DIAGONAL_B 2   // PB2 — TR + BL diagonal
#endif
#ifndef DICE_SEGMENT_MIDDLE_ROW
#define DICE_SEGMENT_MIDDLE_ROW 3   // PB3 — ML + MR middle row
#endif

#define DICE_BUTTON_PIN         4   // PB4 — also the ATtiny13A RESET pin

#define DICE_BIT(bit)           ((uint8_t)(1u << (bit)))

namespace arcdice {

constexpr uint8_t kCenterMask     = DICE_BIT(DICE_SEGMENT_CENTER);
constexpr uint8_t kDiagonalAMask  = DICE_BIT(DICE_SEGMENT_DIAGONAL_A);
constexpr uint8_t kDiagonalBMask  = DICE_BIT(DICE_SEGMENT_DIAGONAL_B);
constexpr uint8_t kMiddleRowMask  = DICE_BIT(DICE_SEGMENT_MIDDLE_ROW);

constexpr uint8_t kButtonMask     = DICE_BIT(DICE_BUTTON_PIN);

constexpr uint8_t kDisplayMask =
    kCenterMask | kDiagonalAMask | kDiagonalBMask | kMiddleRowMask;

// Face indices are 0..6 inclusive. Face 0 is the "all off" sentinel
// (never displayed by the firmware — roll() returns 1..6) but is included
// here so the array length matches the README's 1..6 documentation.
//
// The mapping matches the README "Dice Face Layout" table:
//
//   face 1 = center
//   face 2 = diagonal A
//   face 3 = center + diagonal A           (vertical middle column)
//   face 4 = diagonal A + diagonal B       (full X, four corners)
//   face 5 = center + diagonals A + B      (full X + center)
//   face 6 = diagonals A + B + middle row  (four corners + middle two)
constexpr uint8_t kDiceFaceMasks[7] = {
    0,                                              // face 0 — unused
    kCenterMask,                                    // face 1
    kDiagonalAMask,                                  // face 2
    kCenterMask | kDiagonalAMask,                    // face 3
    kDiagonalAMask | kDiagonalBMask,                 // face 4
    kCenterMask | kDiagonalAMask | kDiagonalBMask,   // face 5
    kDiagonalAMask | kDiagonalBMask | kMiddleRowMask, // face 6
};

// Returns the PORTB mask for the requested face. Faces 1..6 are valid;
// face 0 returns 0 (all segments off). Out-of-range faces return 0xFF
// so a programming error is visible immediately rather than silently
// rolling "all off".
constexpr uint8_t faceMask(uint8_t face) {
    return (face < 7) ? kDiceFaceMasks[face] : 0xFF;
}

}  // namespace arcdice

#endif  // ARCDICE_DICE_H_