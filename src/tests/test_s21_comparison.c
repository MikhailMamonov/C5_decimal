#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"

static void run_compare_equal_test(TestParams *params) {
  int return_code = s21_is_equal(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

static void run_compare_not_equal_test(TestParams *params) {
  int return_code = s21_is_not_equal(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

static void run_compare_less_test(TestParams *params) {
  int return_code = s21_is_less(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

static void run_compare_less_or_equal_test(TestParams *params) {
  int return_code = s21_is_less_or_equal(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

static void run_compare_greater_test(TestParams *params) {
  int return_code = s21_is_greater(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

static void run_compare_greater_or_equal_test(TestParams *params) {
  int return_code = s21_is_greater_or_equal(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}


// ТЕСТЫ ДЛЯ s21_is_equal

COMPARE_EQUAL_TEST_CASES(equal_simple, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 1,
  .test_name = "5 == 5 is TRUE "
})

COMPARE_EQUAL_TEST_CASES(equal_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 1,
  .test_name = "-5 == -5 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_simple, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .value2 = {{0x00000004, 0x00000000, 0x00000000, 0x00000000}}, // 4
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "10 == 4 IS FALSE"
})


COMPARE_EQUAL_TEST_CASES(not_equal_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x0000000C, 0x00000000, 0x00000000, 0x80000000}}, // -12
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 
  .expected_return_code = 0,
  .test_name = "-5 == -12 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_mixed_sign, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 
  .expected_return_code = 0,
  .test_name = "-5 == 5 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(equal_zero_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "+0 == +0 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(equal_zero_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "+0 == -0 is TRUE (critical!)"
})

COMPARE_EQUAL_TEST_CASES(equal_zero_both_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "-0 == -0 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(equal_zero_different_scales, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00030000}}, // 0.000
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00050000}}, // 0.00000
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "0.000 == 0.00000 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_zero_and_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,
  .test_name = "0 == 5 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_zero_and_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,
  .test_name = "0 == -5 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(equal_different_scale_1, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x00000096, 0x00000000, 0x00000000, 0x00020000}}, // 1.50
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "1.5 == 1.50 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(equal_different_scale_2, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "1 == 1.0 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(equal_different_scale_3, {
  .value1 = {{0x00000064, 0x00000000, 0x00000000, 0x00020000}}, // 1.00
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "1.00 == 1 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_different_scale_1, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x00000097, 0x00000000, 0x00000000, 0x00020000}}, // 1.51
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,
  .test_name = "1.5 == 1.51 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_different_scale_2, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00010000}}, // 1.1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,
  .test_name = "1 == 1.1 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(equal_negative_different_scales, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000096, 0x00000000, 0x00000000, 0x80020000}}, // -1.50
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "-1.5 == -1.50 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_negative_different_scales, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000097, 0x00000000, 0x00000000, 0x80020000}}, // -1.51
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,
  .test_name = "-1.5 == -1.51 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(equal_decimal_01, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00020000}}, // 0.10
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "0.1 == 0.10 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(equal_decimal_025, {
  .value1 = {{0x00000019, 0x00000000, 0x00000000, 0x00020000}}, // 0.25
  .value2 = {{0x000000FA, 0x00000000, 0x00000000, 0x00030000}}, // 0.250
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "0.25 == 0.250 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_decimal, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,
  .test_name = "0.1 == 0.11 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(equal_max_value, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "MAX == MAX is TRUE"
})

COMPARE_EQUAL_TEST_CASES(not_equal_near_max, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,
  .test_name = "MAX == MAX-1 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(equal_max_with_scale, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, // MAX (scale 1)
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 0,  // Не равны, т.к. разные значения
  .test_name = "MAX == MAX with scale 1 is FALSE"
})

COMPARE_EQUAL_TEST_CASES(equal_large_scale, {
  .value1 = {{0x0001E240, 0x00000000, 0x00000000, 0x00060000}}, // 123456
  .value2 = {{0x075BCD15, 0x00000000, 0x00000000, 0x00090000}}, // 123456000
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "0.123456 == 0.123456000 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(equal_very_small, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "1e-28 == 1e-28 is TRUE"
})

COMPARE_EQUAL_TEST_CASES(equal_very_small_diff_scale, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x001D0000}}, // 1e-28 (scale 29)
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "1e-28 == 1e-28 (scale 29) is TRUE"
})

// 8. Переполнение при выравнивании
COMPARE_EQUAL_TEST_CASES(equal_no_overflow, {
  .value1 = {{0x19999999, 0x00000000, 0x00000000, 0x00010000}}, // MAX/10
  .value2 = {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020000}}, // MAX/100
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1,
  .test_name = "MAX/10 == MAX/100 is TRUE"
})

// ТЕСТЫ ДЛЯ s21_is_not_equal

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_simple, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "5 != 3 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x0000000C, 0x00000000, 0x00000000, 0x80000000}}, // -12
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 != -12 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_mixed_sign, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 != 5 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_zero_and_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 != 5 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_zero_and_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 != -5 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_different_scale_1, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x00000097, 0x00000000, 0x00000000, 0x00020000}}, // 1.51
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1.5 != 1.51 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_different_scale_2, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00010000}}, // 1.1
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1 != 1.1 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_negative_different_scales, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000097, 0x00000000, 0x00000000, 0x80020000}}, // -1.51
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-1.5 != -1.51 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_decimal, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.1 != 0.11 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_near_max, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX != MAX-1 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_true_max_with_scale, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, // MAX (scale 1)
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX != MAX with scale 1 is TRUE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_simple, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "5 != 5 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 != -5 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_zero_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "+0 != +0 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_zero_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "+0 != -0 is FALSE (critical!)"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_zero_both_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-0 != -0 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_zero_different_scales, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00030000}}, // 0.000
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00050000}}, // 0.00000
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.000 != 0.00000 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_different_scale_1, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x00000096, 0x00000000, 0x00000000, 0x00020000}}, // 1.50
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.5 != 1.50 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_different_scale_2, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1 != 1.0 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_different_scale_3, {
  .value1 = {{0x00000064, 0x00000000, 0x00000000, 0x00020000}}, // 1.00
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.00 != 1 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_negative_different_scales, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000096, 0x00000000, 0x00000000, 0x80020000}}, // -1.50
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-1.5 != -1.50 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_decimal_01, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00020000}}, // 0.10
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.1 != 0.10 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_decimal_025, {
  .value1 = {{0x00000019, 0x00000000, 0x00000000, 0x00020000}}, // 0.25
  .value2 = {{0x000000FA, 0x00000000, 0x00000000, 0x00030000}}, // 0.250
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.25 != 0.250 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_max_value, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX != MAX is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_large_scale, {
  .value1 = {{0x0001E240, 0x00000000, 0x00000000, 0x00060000}}, // 123456
  .value2 = {{0x075BCD15, 0x00000000, 0x00000000, 0x00090000}}, // 123456000
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.123456 != 0.123456000 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_very_small, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1e-28 != 1e-28 is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_very_small_diff_scale, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x001D0000}}, // 1e-28 (scale 29)
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1e-28 != 1e-28 (scale 29) is FALSE"
})

COMPARE_NOT_EQUAL_TEST_CASES(not_equal_false_no_overflow, {
  .value1 = {{0x19999999, 0x00000000, 0x00000000, 0x00010000}}, // MAX/10
  .value2 = {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020000}}, // MAX/100
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX/10 != MAX/100 is FALSE"
})

// Тесты для s21_less
// Позитивные тесты (возвращают 1 - число меньше)
COMPARE_LESS_TEST_CASES(less_true_simple, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "3 < 5 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 < -3 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_mixed_sign_1, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 < 3 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_mixed_sign_2, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 < 5 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_mixed_sign_3, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 < 0 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_different_scale_1, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x000000FA, 0x00000000, 0x00000000, 0x00020000}}, // 2.50
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1.5 < 2.50 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_negative_different_scales, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-1.5 < -1.00 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_decimal, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.1 < 0.11 is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_near_max, {
  .value1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX-1 < MAX is TRUE"
})

COMPARE_LESS_TEST_CASES(less_true_very_small, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 < 1e-28 is TRUE"
})

// Негативные тесты (возвращают 0 - число НЕ меньше)
COMPARE_LESS_TEST_CASES(less_false_simple, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "5 < 3 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "5 < 5 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_negative, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-3 < -5 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_negative_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 < -5 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_mixed_sign_1, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "3 < -5 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_mixed_sign_2, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "5 < 0 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_zero_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "+0 < +0 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_zero_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "+0 < -0 is FALSE (critical!)"
})

COMPARE_LESS_TEST_CASES(less_false_zero_both_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-0 < -0 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_zero_different_scales, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00030000}}, // 0.000
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00050000}}, // 0.00000
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.000 < 0.00000 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_different_scale_1, {
  .value1 = {{0x00000096, 0x00000000, 0x00000000, 0x00020000}}, // 1.50
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.50 < 1.5 is FALSE (equal)"
})

COMPARE_LESS_TEST_CASES(less_false_different_scale_2, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.0 < 1 is FALSE (equal)"
})

COMPARE_LESS_TEST_CASES(less_false_negative_different_scales, {
  .value1 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-1.00 < -1.5 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_decimal, {
  .value1 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.11 < 0.1 is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_max_value, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX < MAX is FALSE"
})

COMPARE_LESS_TEST_CASES(less_false_max_with_scale, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, // MAX (scale 1)
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX with scale 1 < MAX is TRUE (MAX/10 < MAX)"
})

COMPARE_LESS_TEST_CASES(less_false_no_overflow, {
  .value1 = {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020000}}, // MAX/100
  .value2 = {{0x19999999, 0x00000000, 0x00000000, 0x00010000}}, // MAX/10
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX/100 < MAX/10 is FALSE (equal)"
})

COMPARE_LESS_TEST_CASES(less_true_different_scale_2, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.0 < 1 is FALSE (equal)"
})

COMPARE_LESS_TEST_CASES(less_true_large_scale, {
  .value1 = {{0x0001E240, 0x00000000, 0x00000000, 0x00060000}}, // 0.123456
  .value2 = {{0x075BCD15, 0x00000000, 0x00000000, 0x00080000}}, // 0.12345600
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.123456 < 0.12345600 is FALSE (equal)"
})

// s21_less_or_equal
// Позитивные тесты (возвращают 1 - число меньше или равно)
COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_simple_less, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "3 <= 5 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_simple_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "5 <= 5 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_negative_less, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 <= -3 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_negative_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 <= -5 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_mixed_sign_1, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 <= 3 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_mixed_sign_2, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 <= 5 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_mixed_sign_3, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 <= 0 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_zero_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "+0 <= +0 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_zero_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "+0 <= -0 is TRUE (critical!)"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_zero_both_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-0 <= -0 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_zero_different_scales, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00030000}}, // 0.000
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00050000}}, // 0.00000
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.000 <= 0.00000 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_different_scale_less, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x000000FA, 0x00000000, 0x00000000, 0x00020000}}, // 2.50
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1.5 <= 2.50 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_different_scale_equal, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x00000096, 0x00000000, 0x00000000, 0x00020000}}, // 1.50
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1.5 <= 1.50 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_negative_different_scales_less, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-1.5 <= -1.00 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_negative_different_scales_equal, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000096, 0x00000000, 0x00000000, 0x80020000}}, // -1.50
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-1.5 <= -1.50 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_decimal_less, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.1 <= 0.11 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_decimal_equal, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00020000}}, // 0.10
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.1 <= 0.10 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_near_max, {
  .value1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX-1 <= MAX is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_max_equal, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX <= MAX is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_very_small, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 <= 1e-28 is TRUE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_true_large_scale_equal, {
  .value1 = {{0x0001E240, 0x00000000, 0x00000000, 0x00060000}}, // 0.123456
  .value2 = {{0x075BCD15, 0x00000000, 0x00000000, 0x00090000}}, // 0.123456000
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.123456 <= 0.123456000 is TRUE"
})

// Негативные тесты (возвращают 0 - число НЕ меньше или равно)
COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_simple, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "5 <= 3 is FALSE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_negative, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-3 <= -5 is FALSE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_mixed_sign_1, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "3 <= -5 is FALSE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_mixed_sign_2, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "5 <= 0 is FALSE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_different_scale_1, {
  .value1 = {{0x000000FA, 0x00000000, 0x00000000, 0x00020000}}, // 2.50
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "2.50 <= 1.5 is FALSE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_different_scale_2, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1 <= 1.0 is FALSE (equal, should be TRUE!)"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_negative_different_scales, {
  .value1 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-1.00 <= -1.5 is FALSE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_decimal, {
  .value1 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.11 <= 0.1 is FALSE"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_max_with_scale, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, // MAX (scale 1)
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX <= MAX with scale 1 is FALSE (MAX > MAX/10)"
})

COMPARE_LESS_OR_EQUAL_TEST_CASES(less_or_equal_false_no_overflow, {
  .value1 = {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020000}}, // MAX/100
  .value2 = {{0x19999999, 0x00000000, 0x00000000, 0x00010000}}, // MAX/10
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX/100 <= MAX/10 is FALSE (equal)"
})

// greater
// Позитивные тесты (возвращают 1 - число больше)
COMPARE_GREATER_TEST_CASES(greater_true_simple, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "5 > 3 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_negative, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-3 > -5 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_mixed_sign_1, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "3 > -5 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_mixed_sign_2, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "5 > 0 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_mixed_sign_3, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 > -5 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_different_scale_1, {
  .value1 = {{0x000000FA, 0x00000000, 0x00000000, 0x00020000}}, // 2.50
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "2.50 > 1.5 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_negative_different_scales, {
  .value1 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-1.00 > -1.5 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_decimal, {
  .value1 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.11 > 0.1 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_near_max, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX > MAX-1 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_max_with_scale, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, // MAX (scale 1)
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX > MAX with scale 1 is TRUE (MAX > MAX/10)"
})

COMPARE_GREATER_TEST_CASES(greater_true_very_small, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1e-28 > 0 is TRUE"
})

COMPARE_GREATER_TEST_CASES(greater_true_no_overflow, {
  .value1 = {{0x19999999, 0x00000000, 0x00000000, 0x00010000}}, // MAX/10
  .value2 = {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020000}}, // MAX/100
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX/10 > MAX/100 is TRUE"
})

// Негативные тесты (возвращают 0 - число НЕ больше)
COMPARE_GREATER_TEST_CASES(greater_false_simple, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "3 > 5 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "5 > 5 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 > -3 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_negative_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 > -5 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_mixed_sign_1, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 > 3 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_mixed_sign_2, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0 > 5 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_zero_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "+0 > +0 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_zero_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "+0 > -0 is FALSE (critical!)"
})

COMPARE_GREATER_TEST_CASES(greater_false_zero_both_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-0 > -0 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_zero_different_scales, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00030000}}, // 0.000
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00050000}}, // 0.00000
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.000 > 0.00000 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_different_scale_1, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x000000FA, 0x00000000, 0x00000000, 0x00020000}}, // 2.50
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.5 > 2.50 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_different_scale_2, {
  .value1 = {{0x00000096, 0x00000000, 0x00000000, 0x00020000}}, // 1.50
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.50 > 1.5 is FALSE (equal)"
})

COMPARE_GREATER_TEST_CASES(greater_false_negative_different_scales, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-1.5 > -1.00 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_decimal, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.1 > 0.11 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_max_equal, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX > MAX is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_near_max, {
  .value1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX-1 > MAX is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_false_very_small, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0 > 1e-28 is FALSE"
})

COMPARE_GREATER_TEST_CASES(greater_true_different_scale_2, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1 > 1.0 is FALSE (equal)"
})

COMPARE_GREATER_TEST_CASES(greater_true_large_scale, {
  .value1 = {{0x075BCD15, 0x00000000, 0x00000000, 0x00080000}}, // 0.12345600
  .value2 = {{0x0001E240, 0x00000000, 0x00000000, 0x00060000}}, // 0.123456
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.12345600 > 0.123456 is FALSE (equal)"
})

// greater_or_equal
// Позитивные тесты (возвращают 1 - число больше или равно)
COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_simple_greater, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "5 >= 3 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_simple_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "5 >= 5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_negative_greater, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-3 >= -5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_negative_equal, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-5 >= -5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_mixed_sign_1, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "3 >= -5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_mixed_sign_2, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "5 >= 0 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_mixed_sign_3, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0 >= -5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_zero_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "+0 >= +0 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_zero_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // +0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "+0 >= -0 is TRUE (critical!)"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_zero_both_negative, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-0 >= -0 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_zero_different_scales, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00030000}}, // 0.000
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00050000}}, // 0.00000
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.000 >= 0.00000 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_different_scale_greater, {
  .value1 = {{0x000000FA, 0x00000000, 0x00000000, 0x00020000}}, // 2.50
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "2.50 >= 1.5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_different_scale_equal, {
  .value1 = {{0x00000096, 0x00000000, 0x00000000, 0x00020000}}, // 1.50
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1.50 >= 1.5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_negative_different_scales_greater, {
  .value1 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-1.00 >= -1.5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_negative_different_scales_equal, {
  .value1 = {{0x00000096, 0x00000000, 0x00000000, 0x80020000}}, // -1.50
  .value2 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "-1.50 >= -1.5 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_decimal_greater, {
  .value1 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.11 >= 0.1 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_decimal_equal, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00020000}}, // 0.10
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.10 >= 0.1 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_near_max, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX >= MAX-1 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_max_equal, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX >= MAX is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_max_with_scale, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, // MAX (scale 1)
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX >= MAX with scale 1 is TRUE (MAX > MAX/10)"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_very_small, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "1e-28 >= 0 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_large_scale_equal, {
  .value1 = {{0x075BCD15, 0x00000000, 0x00000000, 0x00090000}}, // 0.123456000
  .value2 = {{0x0001E240, 0x00000000, 0x00000000, 0x00060000}}, // 0.123456
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "0.123456000 >= 0.123456 is TRUE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_true_no_overflow, {
  .value1 = {{0x19999999, 0x00000000, 0x00000000, 0x00010000}}, // MAX/10
  .value2 = {{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020000}}, // MAX/100
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 1,
  .test_name = "MAX/10 >= MAX/100 is TRUE"
})

// Негативные тесты (возвращают 0 - число НЕ больше или равно)
COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_simple, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "3 >= 5 is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 >= -3 is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_mixed_sign_1, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 >= 3 is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_mixed_sign_2, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0 >= 5 is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_different_scale_1, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}}, // 1.5
  .value2 = {{0x000000FA, 0x00000000, 0x00000000, 0x00020000}}, // 2.50
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.5 >= 2.50 is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_different_scale_2, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "1.0 >= 1 is FALSE (equal, should be TRUE!)"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_negative_different_scales, {
  .value1 = {{0x0000000F, 0x00000000, 0x00000000, 0x80010000}}, // -1.5
  .value2 = {{0x00000064, 0x00000000, 0x00000000, 0x80020000}}, // -1.00
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-1.5 >= -1.00 is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_decimal, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1
  .value2 = {{0x0000000B, 0x00000000, 0x00000000, 0x00020000}}, // 0.11
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0.1 >= 0.11 is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_near_max, {
  .value1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX - 1
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX-1 >= MAX is FALSE"
})

COMPARE_GREATER_OR_EQUAL_TEST_CASES(greater_or_equal_false_very_small, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0 >= 1e-28 is FALSE"
})

Suite *comparison_suite_create(void) {
    Suite *s = suite_create("s21_comparison");
    
    TCase *tc_equal = tcase_create("is_equal");
    tcase_add_test(tc_equal, test_equal_simple);
    tcase_add_test(tc_equal, test_equal_negative);
    tcase_add_test(tc_equal, test_equal_zero_positive);
    tcase_add_test(tc_equal, test_equal_zero_negative);
    tcase_add_test(tc_equal, test_equal_zero_both_negative);
    tcase_add_test(tc_equal, test_equal_zero_different_scales);
    tcase_add_test(tc_equal, test_equal_different_scale_1);
    tcase_add_test(tc_equal, test_equal_different_scale_2);
    tcase_add_test(tc_equal, test_equal_different_scale_3);
    tcase_add_test(tc_equal, test_equal_negative_different_scales);
    tcase_add_test(tc_equal, test_equal_decimal_01);
    tcase_add_test(tc_equal, test_equal_decimal_025);
    tcase_add_test(tc_equal, test_equal_max_value);
    tcase_add_test(tc_equal, test_equal_max_with_scale);
    tcase_add_test(tc_equal, test_equal_large_scale);
    tcase_add_test(tc_equal, test_equal_very_small);
    tcase_add_test(tc_equal, test_equal_very_small_diff_scale);
    tcase_add_test(tc_equal, test_equal_no_overflow);
    suite_add_tcase(s, tc_equal);

    TCase *tc_not_equal = tcase_create("is_not_equal");
    // Позитивные тесты (возвращают 1)
    tcase_add_test(tc_not_equal, test_not_equal_true_simple);
    tcase_add_test(tc_not_equal, test_not_equal_true_negative);
    tcase_add_test(tc_not_equal, test_not_equal_simple);
    tcase_add_test(tc_not_equal, test_not_equal_true_mixed_sign);
    tcase_add_test(tc_not_equal, test_not_equal_true_zero_and_positive);
    tcase_add_test(tc_not_equal, test_not_equal_true_zero_and_negative);
    tcase_add_test(tc_not_equal, test_not_equal_zero_and_positive);
    tcase_add_test(tc_not_equal, test_not_equal_zero_and_negative);
    tcase_add_test(tc_not_equal, test_not_equal_true_different_scale_1);
    tcase_add_test(tc_not_equal, test_not_equal_true_different_scale_2);
     tcase_add_test(tc_not_equal, test_not_equal_different_scale_1);
    tcase_add_test(tc_not_equal, test_not_equal_different_scale_2);
    tcase_add_test(tc_not_equal, test_not_equal_true_negative_different_scales);
    tcase_add_test(tc_not_equal, test_not_equal_true_decimal);
    tcase_add_test(tc_not_equal, test_not_equal_true_near_max);
    tcase_add_test(tc_not_equal, test_not_equal_true_max_with_scale);
    tcase_add_test(tc_not_equal, test_not_equal_mixed_sign);
    tcase_add_test(tc_not_equal, test_not_equal_negative);
    tcase_add_test(tc_not_equal, test_not_equal_near_max);
    tcase_add_test(tc_not_equal, test_not_equal_decimal);
    tcase_add_test(tc_not_equal, test_not_equal_negative_different_scales);

    // Негативные тесты (возвращают 0)
    tcase_add_test(tc_not_equal, test_not_equal_false_simple);
    tcase_add_test(tc_not_equal, test_not_equal_false_negative);
    tcase_add_test(tc_not_equal, test_not_equal_false_zero_positive);
    tcase_add_test(tc_not_equal, test_not_equal_false_zero_negative);
    tcase_add_test(tc_not_equal, test_not_equal_false_zero_both_negative);
    tcase_add_test(tc_not_equal, test_not_equal_false_zero_different_scales);
    tcase_add_test(tc_not_equal, test_not_equal_false_different_scale_1);
    tcase_add_test(tc_not_equal, test_not_equal_false_different_scale_2);
    tcase_add_test(tc_not_equal, test_not_equal_false_different_scale_3);
    tcase_add_test(tc_not_equal, test_not_equal_false_negative_different_scales);
    tcase_add_test(tc_not_equal, test_not_equal_false_decimal_01);
    tcase_add_test(tc_not_equal, test_not_equal_false_decimal_025);
    tcase_add_test(tc_not_equal, test_not_equal_false_max_value);
    tcase_add_test(tc_not_equal, test_not_equal_false_large_scale);
    tcase_add_test(tc_not_equal, test_not_equal_false_very_small);
    tcase_add_test(tc_not_equal, test_not_equal_false_very_small_diff_scale);
    tcase_add_test(tc_not_equal, test_not_equal_false_no_overflow);
    suite_add_tcase(s, tc_not_equal);

    TCase *tc_less = tcase_create("is_less");
    // Позитивные тесты (возвращают 1)
    tcase_add_test(tc_less, test_less_true_simple);
    tcase_add_test(tc_less, test_less_true_negative);
    tcase_add_test(tc_less, test_less_true_mixed_sign_1);
    tcase_add_test(tc_less, test_less_true_mixed_sign_2);
    tcase_add_test(tc_less, test_less_true_mixed_sign_3);
    tcase_add_test(tc_less, test_less_true_different_scale_1);
    tcase_add_test(tc_less, test_less_true_negative_different_scales);
    tcase_add_test(tc_less, test_less_true_decimal);
    tcase_add_test(tc_less, test_less_true_near_max);
    tcase_add_test(tc_less, test_less_true_very_small);
    tcase_add_test(tc_less, test_less_false_max_with_scale); // Это TRUE
    // ADD the generated less true tests
    tcase_add_test(tc_less, test_less_true_different_scale_2);
    tcase_add_test(tc_less, test_less_true_large_scale);
    // Негативные тесты (возвращают 0)
    tcase_add_test(tc_less, test_less_false_simple);
    tcase_add_test(tc_less, test_less_false_equal);
    tcase_add_test(tc_less, test_less_false_negative);
    tcase_add_test(tc_less, test_less_false_negative_equal);
    tcase_add_test(tc_less, test_less_false_mixed_sign_1);
    tcase_add_test(tc_less, test_less_false_mixed_sign_2);
    tcase_add_test(tc_less, test_less_false_zero_positive);
    tcase_add_test(tc_less, test_less_false_zero_negative);
    tcase_add_test(tc_less, test_less_false_zero_both_negative);
    tcase_add_test(tc_less, test_less_false_zero_different_scales);
    tcase_add_test(tc_less, test_less_false_different_scale_1);
    tcase_add_test(tc_less, test_less_false_different_scale_2);
    tcase_add_test(tc_less, test_less_false_negative_different_scales);
    tcase_add_test(tc_less, test_less_false_decimal);
    tcase_add_test(tc_less, test_less_false_max_value);
    tcase_add_test(tc_less, test_less_false_no_overflow);
    suite_add_tcase(s, tc_less);
    
    // ---- Тесты для s21_is_less_or_equal ----
    TCase *tc_less_or_equal = tcase_create("is_less_or_equal");
    // Позитивные тесты (возвращают 1)
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_simple_less);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_simple_equal);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_negative_less);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_negative_equal);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_mixed_sign_1);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_mixed_sign_2);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_mixed_sign_3);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_zero_positive);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_zero_negative);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_zero_both_negative);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_zero_different_scales);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_different_scale_less);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_different_scale_equal);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_negative_different_scales_less);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_negative_different_scales_equal);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_decimal_less);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_decimal_equal);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_near_max);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_max_equal);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_very_small);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_true_large_scale_equal);
    // ADD the generated less_or_equal test
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_different_scale_2);
    // Негативные тесты (возвращают 0)
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_simple);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_negative);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_mixed_sign_1);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_mixed_sign_2);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_different_scale_1);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_negative_different_scales);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_decimal);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_max_with_scale);
    tcase_add_test(tc_less_or_equal, test_less_or_equal_false_no_overflow);
    suite_add_tcase(s, tc_less_or_equal);

    // ---- Тесты для s21_is_greater ----
    TCase *tc_greater = tcase_create("is_greater");
    // Позитивные тесты (возвращают 1)
    tcase_add_test(tc_greater, test_greater_true_simple);
    tcase_add_test(tc_greater, test_greater_true_negative);
    tcase_add_test(tc_greater, test_greater_true_mixed_sign_1);
    tcase_add_test(tc_greater, test_greater_true_mixed_sign_2);
    tcase_add_test(tc_greater, test_greater_true_mixed_sign_3);
    tcase_add_test(tc_greater, test_greater_true_different_scale_1);
    tcase_add_test(tc_greater, test_greater_true_negative_different_scales);
    tcase_add_test(tc_greater, test_greater_true_decimal);
    tcase_add_test(tc_greater, test_greater_true_near_max);
    tcase_add_test(tc_greater, test_greater_true_max_with_scale);
    tcase_add_test(tc_greater, test_greater_true_very_small);
    tcase_add_test(tc_greater, test_greater_true_no_overflow);
    // ADD the generated greater true tests
    tcase_add_test(tc_greater, test_greater_true_different_scale_2);
    tcase_add_test(tc_greater, test_greater_true_large_scale);
    // Негативные тесты (возвращают 0)
    tcase_add_test(tc_greater, test_greater_false_simple);
    tcase_add_test(tc_greater, test_greater_false_equal);
    tcase_add_test(tc_greater, test_greater_false_negative);
    tcase_add_test(tc_greater, test_greater_false_negative_equal);
    tcase_add_test(tc_greater, test_greater_false_mixed_sign_1);
    tcase_add_test(tc_greater, test_greater_false_mixed_sign_2);
    tcase_add_test(tc_greater, test_greater_false_zero_positive);
    tcase_add_test(tc_greater, test_greater_false_zero_negative);
    tcase_add_test(tc_greater, test_greater_false_zero_both_negative);
    tcase_add_test(tc_greater, test_greater_false_zero_different_scales);
    tcase_add_test(tc_greater, test_greater_false_different_scale_1);
    tcase_add_test(tc_greater, test_greater_false_different_scale_2);
    tcase_add_test(tc_greater, test_greater_false_negative_different_scales);
    tcase_add_test(tc_greater, test_greater_false_decimal);
    tcase_add_test(tc_greater, test_greater_false_max_equal);
    tcase_add_test(tc_greater, test_greater_false_near_max);
    tcase_add_test(tc_greater, test_greater_false_very_small);
    suite_add_tcase(s, tc_greater);
    
    // ---- Тесты для s21_is_greater_or_equal ----
    TCase *tc_greater_or_equal = tcase_create("is_greater_or_equal");
    // Позитивные тесты (возвращают 1)
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_simple_greater);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_simple_equal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_negative_greater);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_negative_equal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_mixed_sign_1);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_mixed_sign_2);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_mixed_sign_3);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_zero_positive);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_zero_negative);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_zero_both_negative);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_zero_different_scales);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_different_scale_greater);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_different_scale_equal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_negative_different_scales_greater);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_negative_different_scales_equal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_decimal_greater);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_decimal_equal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_near_max);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_max_equal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_max_with_scale);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_very_small);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_large_scale_equal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_true_no_overflow);
    // ADD the generated greater_or_equal test
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_different_scale_2);
    // Негативные тесты (возвращают 0)
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_simple);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_negative);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_mixed_sign_1);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_mixed_sign_2);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_different_scale_1);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_negative_different_scales);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_decimal);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_near_max);
    tcase_add_test(tc_greater_or_equal, test_greater_or_equal_false_very_small);
    suite_add_tcase(s, tc_greater_or_equal);
    
    return s;
}

