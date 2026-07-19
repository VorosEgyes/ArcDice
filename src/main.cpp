#include <Arduino.h>
#include <avr/sleep.h>
#include "dice.h"

// LED group layout on the 3x3 dice face:
//
//   TL  TM  TR
//   ML  CC  MR        CC   = center       (SEGMENT_CENTER)
//   BL  BM  BR        ML/MR = middle row  (SEGMENT_MIDDLE_ROW)
//                      TL/BR = diagonal A  (SEGMENT_DIAGONAL_A)
//                      TR/BL = diagonal B  (SEGMENT_DIAGONAL_B)
//
// NOTE: PB4 is the ATtiny13A RESET pin. Using it as a button input
// requires the RSTDISBL fuse to be programmed, which DISABLES ISP
// reprogramming -- a high-voltage programmer is then required to flash.
//
// The constants below come from include/dice.h so the host-native test
// runner can validate face-mask logic without an AVR toolchain.
using arcdice::kDisplayMask;
using arcdice::kButtonMask;
using arcdice::kDiceFaceMasks;

volatile bool buttonPressed = false;
volatile bool buttonInterruptPending = false;
volatile uint8_t randomEntropy = 0;
volatile uint8_t edgeCounter = 0;
volatile uint8_t prngState = 0x5A;

void showFace(uint8_t face) {
  PORTB = (PORTB & ~kDisplayMask) | kDiceFaceMasks[face];
}

uint8_t nextRandomByte() {
  uint8_t lsb = prngState & 1;
  prngState >>= 1;
  if (lsb) {
    prngState ^= 0xB8;
  }
  return prngState;
}

void mixEntropy(uint8_t entropy) {
  prngState ^= entropy ^ TCNT0;
  if (!prngState) {
    prngState = 0x5A;
  }
}

uint8_t randomFace() {
  return (nextRandomByte() % 6) + 1;
}

void runStartupTest() {
  for (uint8_t face = 1; face <= 6; ++face) {
    showFace(face);
    delay(110);
  }
  // "All-segments-on" flash: lights every LED group at once for 200 ms.
  // A dead segment or swapped wiring is immediately obvious this way,
  // whereas cycling 1..6 hides a fault that overlaps a working face.
  PORTB |= kDisplayMask;
  delay(200);
  PORTB &= ~kDisplayMask;
}

void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  // Disable BOD during sleep to minimize power draw (~25 uA savings).
  // Must be called within 3 cycles of sleep_enable() per datasheet.
  sleep_bod_disable();
  // Atomic-style sleep: enable interrupts, then sleep, then disable.
  // Using sleep_mode() (which itself does sei+sleep_cpu+cli) races with
  // a pending PCINT and can return immediately. Explicit form is safer.
  sei();
  sleep_cpu();
  sleep_disable();
}

void setup() {
  DDRB |= kDisplayMask;
  DDRB &= ~kButtonMask;
  PORTB |= kButtonMask; // Enable internal pull-up for button

  GIMSK |= (1 << PCIE); // Pin Change Interrupt Enable
  PCMSK |= kButtonMask; // Enable interrupt for the button pin
  sei(); // Enable global interrupts

  mixEntropy(TCNT0);
  PORTB &= ~kDisplayMask; // Ensure all display groups are off at start
  runStartupTest();
}

void loop() {
  if (buttonInterruptPending) {
    noInterrupts();
    buttonInterruptPending = false;
    uint8_t entropy = randomEntropy;
    interrupts();

    delay(25);
    if (!(PINB & kButtonMask)) {
      mixEntropy((entropy << 1) ^ PINB);
      buttonPressed = true;
    }
  }

  if (buttonPressed) {
    buttonPressed = false;
    uint8_t rollCount = 5 + (nextRandomByte() % 7);
    for (uint8_t i = 0; i <= rollCount; ++i) {
      showFace(randomFace());
      delay(60-i*4);
    }
    // Last iteration already showed the result; no need to roll once more.
    delay(450); // Keep the result visible briefly
    PORTB &= ~kDisplayMask; // Turn off all display groups
  }

  if (!buttonPressed && !buttonInterruptPending) {
    goToSleep(); // Go to sleep mode
  }
}

// Accumulate entropy across multiple edges instead of overwriting on
// each interrupt. A single TCNT0 sample is weak; XORing PINB and
// counting edges gives more bits per button press.
ISR(PCINT0_vect) {
  if (!(PINB & kButtonMask)) {
    randomEntropy ^= TCNT0;
    randomEntropy ^= PINB;
    edgeCounter++;
    buttonInterruptPending = true;
  }
}