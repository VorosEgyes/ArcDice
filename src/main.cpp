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
// The constants below come from include/dice.h so the host-native test
// runner can validate face-mask logic without an AVR toolchain.
using arcdice::kDisplayMask;
using arcdice::kButtonMask;
using arcdice::kDiceFaceMasks;
using arcdice::kCenterMask;
using arcdice::kDiagonalAMask;
using arcdice::kDiagonalBMask;
using arcdice::kMiddleRowMask;

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

// Waits up to `ms`, 1 ms at a time, so a fresh button press can interrupt
// an in-progress animation immediately instead of waiting for it to finish.
bool waitOrPressed(uint16_t ms) {
  for (uint16_t i = 0; i < ms; ++i) {
    delay(1);
    if (buttonInterruptPending && !(PINB & kButtonMask)) {
      return true;
    }
  }
  return false;
}

// Number of spin steps in a real roll (see performRoll()).
const uint8_t kDemoSpinCount = 8;

void runStartupTest() {
  // Running-light demo: chases through the 4 LED groups starting top-right
  // (diagonal B), 3 full laps around, then goes dark.
  const uint8_t sequence[4] = {
    kDiagonalBMask, kMiddleRowMask, kDiagonalAMask, kCenterMask
  };
  for (uint8_t lap = 0; lap < 3; ++lap) {
    for (uint8_t i = 0; i < 4; ++i) {
      PORTB = (PORTB & ~kDisplayMask) | sequence[i];
      delay(20);
    }
  }
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

// Runs the spin animation and result display. A fresh button press at any
// point (mid-spin or while the result is shown) restarts the roll right away.
void performRoll() {
  const uint16_t kRollStepMs = 30; // Constant spin speed, no slow-down.
  bool restart = true;
  while (restart) {
    restart = false;
    buttonInterruptPending = false;
    uint8_t rollCount = kDemoSpinCount * 2; // Roll spins twice as long as the demo.
    for (uint8_t i = 0; i <= rollCount; ++i) {
      showFace(randomFace());
      if (waitOrPressed(kRollStepMs)) {
        restart = true;
        break;
      }
    }
    // Last iteration already showed the result; no need to roll once more.
    if (!restart && waitOrPressed(1200)) { // Keep the result visible briefly
      restart = true;
    }
  }
  PORTB &= ~kDisplayMask; // Turn off all display groups
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
      performRoll();
    }
  }

  if (!buttonInterruptPending) {
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