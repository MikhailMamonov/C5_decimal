#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"

static void run_sub_test(TestParams *params) {
  s21_decimal result;

  result.bits[0] = result.bits[1] = result.bits[2] = result.bits[3] = 0;

  int return_code = s21_sub(params->value1, params->value2, &result);

  ck_assert_int_eq(return_code, params->expected_return_code);

  if(return_code==SUCCESS){
    ck_assert_msg(compare_decimal(result, params->expected_result),
        "test '%s' failed: result not match with expected.",
        params->test_name);
  }
  printf("[PASS] %s\n", params->test_name);
}

SUB_TEST_CASES(sub_two_positives_basic, {
  .value1 = {{0x00000019, 0x00000000, 0x00000000, 0x00000000}}, // 25
  .value2 = {{0x00000007, 0x00000000, 0x00000000, 0x00000000}}, // 7
  .expected_result = {{0x00000012, 0x00000000, 0x00000000, 0x00000000}}, // 18
  .expected_return_code = 0,
  .test_name = "25 - 7 = 18"
})

SUB_TEST_CASES(sub_result_goes_negative, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .value2 = {{0x0000001E, 0x00000000, 0x00000000, 0x00000000}}, // 30
  .expected_result = {{0x00000014, 0x00000000, 0x00000000, 0x80000000}}, // -20
  .expected_return_code = 0,
  .test_name = "10 - 30 = -20"
})

SUB_TEST_CASES(sub_two_negatives, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80000000}}, // -15
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x0000000A, 0x00000000, 0x00000000, 0x80000000}}, // -10
  .expected_return_code = 0,
  .test_name = "(-15) - (-5) = -10"
})

SUB_TEST_CASES(sub_different_scales_simple, {
  .value1 = {{0x0000002A, 0x00000000, 0x00000000, 0x00010000}}, // 4.2 (scale 1, мантисса 42)
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2.0 (scale 0, мантисса 2)
  .expected_result = {{0x00000016, 0x00000000, 0x00000000, 0x00010000}}, // 2.2 (scale 1, мантисса 22)
  .expected_return_code = 0,
  .test_name = "4.2 - 2 = 2.2"
})

SUB_TEST_CASES(sub_zero_result_preserves_scale, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00020000}}, // 0.05 (scale 2)
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00020000}}, // 0.05 (scale 2)
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00020000}}, // 0.00 (scale 2)
  .expected_return_code = 0,
  .test_name = "0.05 - 0.05 = 0.00"
})

SUB_TEST_CASES(sub_bankers_rounding_down, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00000000}}, // 15
  .value2 = {{0x0000000E, 0x00000000, 0x00000000, 0x001C0000}}, // 0.0000000000000000000000000014 (scale 28)
  .expected_result = {{0x0000000F, 0x00000000, 0x00000000, 0x00000000}}, // 15 (усекается без округления вверх)
  .expected_return_code = 0,
  .test_name = "15 - 1.4e-26 = 15"
})

SUB_TEST_CASES(sub_bankers_rounding_to_even, {
  // value1 = MAX_DECIMAL (scale 0) — его невозможно умножить на 10!
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, 
  // value2 = 1.5 (мантисса 15, scale 1) — ему придется уменьшать масштаб до 0
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, 
  // В процессе выравнивания 1.5 округлится вверх до 2 (так как 1 — нечетное)
  // Ожидаемый результат: MAX_DECIMAL - 2
  .expected_result = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, 
  .expected_return_code = 0,
  .test_name = "MAX_DECIMAL - 1.5 (1.5 округляется до 2 из-за невозможности выровнять scale)"
})


SUB_TEST_CASES(sub_overflow_to_negative_infinity, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}, // -MAX_DECIMAL
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, 
  .expected_return_code = 2, // FAIL_TOO_SMALL
  .test_name = "(-MAX_DECIMAL) - 1 = Underflow"
})


SUB_TEST_CASES(sub_mixed_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x0000000C, 0x00000000, 0x00000000, 0x80000000}}, // -12
  .expected_result = {{0x00000011, 0x00000000, 0x00000000, 0x00000000}}, // 17
  .expected_return_code = 0,
  .test_name = "5 - (-12) = 17"
})

SUB_TEST_CASES(sub_minus_zero_and_scale, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80020000}}, // -0.05 (scale 2)
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0 (scale 0)
  .expected_result = {{0x00000005, 0x00000000, 0x00000000, 0x80020000}}, // -0.05 (scale 2)
  .expected_return_code = 0,
  .test_name = "-0.05 - 0 = -0.05"
})

SUB_TEST_CASES(sub_from_zero_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0 (scale 0)
  .value2 = {{0x00000037, 0x00000000, 0x00000000, 0x80010000}}, // -5.5 (scale 1, мантисса 55)
  .expected_result = {{0x00000037, 0x00000000, 0x00000000, 0x00010000}}, // 5.5 (scale 1, мантисса 55)
  .expected_return_code = 0,
  .test_name = "0 - (-5.5) = 5.5"
})

SUB_TEST_CASES(sub_max_scale_difference, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10 (scale 0)
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x001C0000}}, // 2e-28 (scale 28)
  .expected_result = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10 (scale 0, усечение без округления)
  .expected_return_code = 0,
  .test_name = "10 - 2e-28 = 10"
})

SUB_TEST_CASES(sub_extreme_small_zero, {
  .value1 = {{0x00000002, 0x00000000, 0x00000000, 0x001C0000}}, // 2e-28 (scale 28)
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x001C0000}}, // 2e-28 (scale 28)
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x001C0000}}, // 0e-28 (scale 28, сохранение экспоненты)
  .expected_return_code = 0,
  .test_name = "2e-28 - 2e-28 = 0e-28"
})

SUB_TEST_CASES(sub_overflow_minus_max, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}, // -MAX_DECIMAL
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // +1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, 
  .expected_return_code = 2, // FAIL_TOO_SMALL (уход в минус бесконечность)
  .test_name = "-MAX_DECIMAL - 1 = Underflow"
})



Suite *sub_suite_create(void) {
  Suite *s = suite_create("Substract");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_sub_two_positives_basic);
  tcase_add_test(tc, test_sub_result_goes_negative);
  tcase_add_test(tc, test_sub_two_negatives);
  tcase_add_test(tc, test_sub_different_scales_simple);
  tcase_add_test(tc, test_sub_zero_result_preserves_scale);
  tcase_add_test(tc, test_sub_bankers_rounding_down);
  tcase_add_test(tc, test_sub_bankers_rounding_to_even);
  tcase_add_test(tc, test_sub_result_goes_negative);
  tcase_add_test(tc, test_sub_overflow_to_negative_infinity);
  tcase_add_test(tc, test_sub_mixed_negative);
  tcase_add_test(tc, test_sub_minus_zero_and_scale);
  tcase_add_test(tc, test_sub_from_zero_negative);
  tcase_add_test(tc, test_sub_max_scale_difference);
  tcase_add_test(tc, test_sub_extreme_small_zero);
  tcase_add_test(tc, test_sub_overflow_minus_max);
  suite_add_tcase(s, tc);
  return s;
}
