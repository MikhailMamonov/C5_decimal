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

// Основные случаи: разный регистр, цифры, символы
COMPARE_TEST_CASES(equal_simple, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 1,
  .test_name = "5 == 5 is TRUE "
})

COMPARE_TEST_CASES(equal_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-5 == -5 is TRUE"
})

COMPARE_TEST_CASES(not_equal_simple, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .value2 = {{0x00000004, 0x00000000, 0x00000000, 0x00000000}}, // 4
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "10 == 4 IS FALSE"
})


COMPARE_TEST_CASES(not_equal_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x0000000C, 0x00000000, 0x00000000, 0x80000000}}, // -12
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 
  .expected_return_code = 0,
  .test_name = "-5 == -12 is FALSE"
})

COMPARE_TEST_CASES(not_equal_mixed_sign, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // -5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 
  .expected_return_code = 0,
  .test_name = "-5 == 5 is FALSE"
})



Suite *to_lower_suite_create(void) {
  Suite *s = suite_create("to_lower");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_sub_positive);
  tcase_add_test(tc, test_sub_negative);
  tcase_add_test(tc, test_sub_mixed_negative);
  tcase_add_test(tc, test_sub_mixed_positive);
  suite_add_tcase(s, tc);
  return s;
}
