// test_dice_masks.cpp — host-native unit tests for include/dice.h.
//
// Run with: pio test -e native
//
// These tests validate the dice face-mapping table against the README
// "Dice Face Layout" specification. They run on the host (Linux/Mac/Windows)
// without an AVR toolchain, so a CI run completes in seconds and a
// regression in the face table is caught before it ships.

#include <stdint.h>
#include <unity.h>

#include "dice.h"

using arcdice::kCenterMask;
using arcdice::kDiagonalAMask;
using arcdice::kDiagonalBMask;
using arcdice::kMiddleRowMask;
using arcdice::kButtonMask;
using arcdice::kDisplayMask;
using arcdice::kDiceFaceMasks;
using arcdice::faceMask;

void setUp(void) {}
void tearDown(void) {}

// --- Bit-position and mask-shape invariants ---------------------------------

void test_button_pin_does_not_overlap_display(void) {
    // PB4 (button) must be entirely separate from PB0..PB3 (display).
    // If they overlapped, lighting a face would also drive the button
    // line, or pressing the button would light an LED.
    TEST_ASSERT_EQUAL_UINT8(0, kButtonMask & kDisplayMask);
}

void test_display_mask_covers_all_four_segments(void) {
    // PB0 | PB1 | PB2 | PB3 = 0b00001111 = 0x0F
    TEST_ASSERT_EQUAL_UINT8(0x0F, kDisplayMask);
}

void test_each_segment_mask_is_a_single_bit(void) {
    // Each segment mask should set exactly one bit. If two segments
    // shared a mask, lighting one would light the other too.
    TEST_ASSERT_EQUAL_UINT8(1, __builtin_popcount(kCenterMask));
    TEST_ASSERT_EQUAL_UINT8(1, __builtin_popcount(kDiagonalAMask));
    TEST_ASSERT_EQUAL_UINT8(1, __builtin_popcount(kDiagonalBMask));
    TEST_ASSERT_EQUAL_UINT8(1, __builtin_popcount(kMiddleRowMask));
}

void test_segment_masks_are_distinct(void) {
    // No two segment masks should be equal, and no bit position should
    // be used by two segments.
    TEST_ASSERT_NOT_EQUAL(kCenterMask, kDiagonalAMask);
    TEST_ASSERT_NOT_EQUAL(kCenterMask, kDiagonalBMask);
    TEST_ASSERT_NOT_EQUAL(kCenterMask, kMiddleRowMask);
    TEST_ASSERT_NOT_EQUAL(kDiagonalAMask, kDiagonalBMask);
    TEST_ASSERT_NOT_EQUAL(kDiagonalAMask, kMiddleRowMask);
    TEST_ASSERT_NOT_EQUAL(kDiagonalBMask, kMiddleRowMask);
}

void test_button_mask_is_pb4(void) {
    // PB4 = bit 4 = 0b00010000 = 0x10
    TEST_ASSERT_EQUAL_UINT8(0x10, kButtonMask);
}

// --- Dice face mask table ---------------------------------------------------
//
// These mirror the README "Dice Face Layout" table:
//
//   1 = center
//   2 = diagonal A
//   3 = center + diagonal A
//   4 = diagonal A + diagonal B
//   5 = center + diagonal A + diagonal B
//   6 = diagonal A + diagonal B + middle row

void test_face_1_is_center_only(void) {
    TEST_ASSERT_EQUAL_UINT8(kCenterMask, kDiceFaceMasks[1]);
    TEST_ASSERT_EQUAL_UINT8(kCenterMask, faceMask(1));
}

void test_face_2_is_diagonal_a_only(void) {
    TEST_ASSERT_EQUAL_UINT8(kDiagonalAMask, kDiceFaceMasks[2]);
    TEST_ASSERT_EQUAL_UINT8(kDiagonalAMask, faceMask(2));
}

void test_face_3_is_center_plus_diagonal_a(void) {
    TEST_ASSERT_EQUAL_UINT8(kCenterMask | kDiagonalAMask, kDiceFaceMasks[3]);
    TEST_ASSERT_EQUAL_UINT8(kCenterMask | kDiagonalAMask, faceMask(3));
}

void test_face_4_is_diagonals_a_and_b(void) {
    TEST_ASSERT_EQUAL_UINT8(kDiagonalAMask | kDiagonalBMask, kDiceFaceMasks[4]);
    TEST_ASSERT_EQUAL_UINT8(kDiagonalAMask | kDiagonalBMask, faceMask(4));
}

void test_face_5_is_center_plus_both_diagonals(void) {
    TEST_ASSERT_EQUAL_UINT8(
        kCenterMask | kDiagonalAMask | kDiagonalBMask, kDiceFaceMasks[5]);
    TEST_ASSERT_EQUAL_UINT8(
        kCenterMask | kDiagonalAMask | kDiagonalBMask, faceMask(5));
}

void test_face_6_is_diagonals_plus_middle_row(void) {
    TEST_ASSERT_EQUAL_UINT8(
        kDiagonalAMask | kDiagonalBMask | kMiddleRowMask, kDiceFaceMasks[6]);
    TEST_ASSERT_EQUAL_UINT8(
        kDiagonalAMask | kDiagonalBMask | kMiddleRowMask, faceMask(6));
}

void test_face_0_is_off(void) {
    // Face 0 is the "all off" sentinel — never displayed by roll(),
    // but the array entry must be exactly zero so a stray index 0
    // doesn't light anything.
    TEST_ASSERT_EQUAL_UINT8(0, kDiceFaceMasks[0]);
    TEST_ASSERT_EQUAL_UINT8(0, faceMask(0));
}

// --- Invariants across the full table --------------------------------------

void test_no_face_lights_a_segment_outside_display_mask(void) {
    for (uint8_t face = 1; face <= 6; ++face) {
        uint8_t m = kDiceFaceMasks[face];
        // Every bit set in a face mask must also be set in kDisplayMask.
        // Otherwise the firmware would drive a non-display pin.
        TEST_ASSERT_EQUAL_UINT8(0, m & ~kDisplayMask);
    }
}

void test_no_face_lights_the_button_pin(void) {
    // A face must never include the button bit. If it did, "showing"
    // a face would also drive the button line low or high.
    for (uint8_t face = 0; face <= 6; ++face) {
        TEST_ASSERT_EQUAL_UINT8(0, kDiceFaceMasks[face] & kButtonMask);
    }
}

void test_all_seven_faces_are_distinct(void) {
    // Two faces producing the same mask would be a programming error
    // that would silently pass at runtime.
    for (uint8_t i = 0; i < 7; ++i) {
        for (uint8_t j = i + 1; j < 7; ++j) {
            TEST_ASSERT_NOT_EQUAL_MESSAGE(
                kDiceFaceMasks[i], kDiceFaceMasks[j],
                "Two faces have identical masks");
        }
    }
}

void test_face_mask_function_handles_out_of_range(void) {
    // Out-of-range faces must NOT silently return 0 (which would look
    // like a successful "all-off" and hide a programming error). The
    // helper returns 0xFF to make the bug visible immediately.
    TEST_ASSERT_EQUAL_UINT8(0xFF, faceMask(7));
    TEST_ASSERT_EQUAL_UINT8(0xFF, faceMask(42));
    TEST_ASSERT_EQUAL_UINT8(0xFF, faceMask(255));
}

// --- PRNG determinism (host-side simulation) --------------------------------
//
// The firmware PRNG is xorshift-step-on-LSB with polynomial 0xB8.
// We can't run the actual ISR on the host, but we can verify that the
// sequence is deterministic and not degenerate (all zeros, period 1).

void test_prng_state_is_nonzero_after_seed(void) {
    // After the firmware initializes prngState = 0x5A, a single call to
    // the equivalent mix step must leave the state nonzero. A zero state
    // would re-seed to 0x5A, making "nextRandomByte() % 6" repeat.
    uint8_t state = 0x5A;
    uint8_t lsb = state & 1;
    state >>= 1;
    if (lsb) {
        state ^= 0xB8;
    }
    TEST_ASSERT_NOT_EQUAL(0, state);
}

void test_prng_sequence_produces_diverse_outputs(void) {
    // The face output is (nextRandomByte() % 6) + 1. Across many calls,
    // the distribution should not collapse to a single value (which would
    // happen if the state were stuck or the polynomial were wrong).
    uint8_t state = 0x5A;
    uint8_t counts[7] = {0};
    for (int i = 0; i < 600; ++i) {
        uint8_t lsb = state & 1;
        state >>= 1;
        if (lsb) {
            state ^= 0xB8;
        }
        uint8_t face = (state % 6) + 1;
        counts[face]++;
    }
    // Each face 1..6 should appear at least once across 600 trials.
    // (Statistical: with a uniform PRNG and 100 samples per face expected,
    // the chance of a face being zero is negligible.)
    for (uint8_t face = 1; face <= 6; ++face) {
        TEST_ASSERT_GREATER_THAN_UINT8_MESSAGE(
            0, counts[face],
            "A face never appeared in 600 PRNG draws");
    }
}

// --- Test runner ------------------------------------------------------------

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();
    RUN_TEST(test_button_pin_does_not_overlap_display);
    RUN_TEST(test_display_mask_covers_all_four_segments);
    RUN_TEST(test_each_segment_mask_is_a_single_bit);
    RUN_TEST(test_segment_masks_are_distinct);
    RUN_TEST(test_button_mask_is_pb4);
    RUN_TEST(test_face_1_is_center_only);
    RUN_TEST(test_face_2_is_diagonal_a_only);
    RUN_TEST(test_face_3_is_center_plus_diagonal_a);
    RUN_TEST(test_face_4_is_diagonals_a_and_b);
    RUN_TEST(test_face_5_is_center_plus_both_diagonals);
    RUN_TEST(test_face_6_is_diagonals_plus_middle_row);
    RUN_TEST(test_face_0_is_off);
    RUN_TEST(test_no_face_lights_a_segment_outside_display_mask);
    RUN_TEST(test_no_face_lights_the_button_pin);
    RUN_TEST(test_all_seven_faces_are_distinct);
    RUN_TEST(test_face_mask_function_handles_out_of_range);
    RUN_TEST(test_prng_state_is_nonzero_after_seed);
    RUN_TEST(test_prng_sequence_produces_diverse_outputs);
    return UNITY_END();
}