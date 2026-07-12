#include <Arduino.h>
#include <avr/sleep.h>

// The 7 visible dice LEDs are wired as 4 logical groups:
// - one center LED
// - one top-left to bottom-right diagonal pair
// - one top-right to bottom-left diagonal pair
// - one middle-row pair
#define SEGMENT_CENTER PB0
#define SEGMENT_DIAGONAL_A PB1
#define SEGMENT_DIAGONAL_B PB2
#define SEGMENT_MIDDLE_ROW PB3
#define BUTTON_PIN PB4

const uint8_t kCenterMask = _BV(SEGMENT_CENTER);
const uint8_t kDiagonalAMask = _BV(SEGMENT_DIAGONAL_A);
const uint8_t kDiagonalBMask = _BV(SEGMENT_DIAGONAL_B);
const uint8_t kMiddleRowMask = _BV(SEGMENT_MIDDLE_ROW);
const uint8_t kDisplayMask = kCenterMask | kDiagonalAMask | kDiagonalBMask | kMiddleRowMask;

const uint8_t diceFaceMasks[7] = {
  0,
  kCenterMask,
  kDiagonalAMask,
  kCenterMask | kDiagonalAMask,
  kDiagonalAMask | kDiagonalBMask,
  kCenterMask | kDiagonalAMask | kDiagonalBMask,
  kDiagonalAMask | kDiagonalBMask | kMiddleRowMask,
};

volatile bool buttonPressed = false;
volatile bool buttonInterruptPending = false;
volatile uint8_t randomEntropy = 0;
uint8_t prngState = 0x5A;

void showFace(uint8_t face) {
  PORTB = (PORTB & ~kDisplayMask) | diceFaceMasks[face];
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
    delay(120);
  }
  PORTB &= ~kDisplayMask;
}

void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  // MCU wakes up here after interrupt
  sleep_disable();
}

void setup() {
  DDRB |= kDisplayMask;
  DDRB &= ~(1 << BUTTON_PIN);
  PORTB |= (1 << BUTTON_PIN); // Enable internal pull-up for button

  GIMSK |= (1 << PCIE); // Pin Change Interrupt Enable
  PCMSK |= (1 << BUTTON_PIN); // Enable interrupt for the button pin
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
    if (!(PINB & (1 << BUTTON_PIN))) {
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
    showFace(randomFace());
    delay(450); // Keep the result visible briefly
    PORTB &= ~kDisplayMask; // Turn off all display groups
  }

  if (!buttonPressed && !buttonInterruptPending) {
    goToSleep(); // Go to sleep mode
  }
}

ISR(PCINT0_vect) {
  if (!(PINB & (1 << BUTTON_PIN))) {
    randomEntropy = TCNT0;
    buttonInterruptPending = true;
  }
}
