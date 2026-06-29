#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"


static void run_add_test(addParams *params) {
  s21_decimal result = {0};

  int return_code = s21_add(params->value1, params->value2, &result);

  ck_assert_int_eq(return_code, params->expected_return_code);
  
  char str_res[128] = {0};
  char str_exp[128] = {0};

  decimal_to_string(result, str_res);
  decimal_to_string(params->expected_result, str_exp);

  ck_assert_msg(compare_decimal(result, params->expected_result),
      " FAIL [%s]:\n result_code = '%d' expected_return_code = '%d'\n result = '%s'\n expected = '%s'.",
      params->test_name, return_code, params->expected_return_code, str_res, str_exp);

  printf("[PASS] %s\n", params->test_name);
}

// Простые математические операции (Scale = 0)

ADD_TEST_CASES(add_zero_is_positive, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0 + 0 = (positive) 0"
})

ADD_TEST_CASES(add_zero, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_return_code = 0,
  .test_name = "1 + 0 = 1"
})

ADD_TEST_CASES(add_positive, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2
  .expected_result = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_return_code = 0,
  .test_name = "1 + 2 = 3"
})

ADD_TEST_CASES(add_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .expected_result = {{0x00000008, 0x00000000, 0x00000000, 0x80000000}}, // -8
  .expected_return_code = 0,
  .test_name = "-5 + (-3) = -8"
})

ADD_TEST_CASES(add_mixed_positive, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .value2 = {{0x00000004, 0x00000000, 0x00000000, 0x80000000}}, // -4
  .expected_result = {{0x00000006, 0x00000000, 0x00000000, 0x00000000}}, // 6
  .expected_return_code = 0,
  .test_name = "10 + (-4) = 6"
})


ADD_TEST_CASES(add_mixed_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x0000000C, 0x00000000, 0x00000000, 0x80000000}}, // -12
  .expected_result = {{0x00000007, 0x00000000, 0x00000000, 0x80000000}}, // -7
  .expected_return_code = 0,
  .test_name = "5 + (-12) = -7"
})

ADD_TEST_CASES(add_big_numbers_without_overflow_0_to_1, {
  .value1 = {{MAX_MASK, 0x00000000, 0x00000000, 0x00000000}}, // MAX_UINT32
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000001, 0x00000000, 0x00000000}}, // 2^32
  .expected_return_code = 0,
  .test_name = "Carry from bits[0] to bits[1]"
})

ADD_TEST_CASES(add_big_numbers_without_overflow_1_to_2, {
  .value1 = {{MAX_MASK, MAX_MASK, 0x00000000, 0x00000000}}, // 2^64 - 1
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000001, 0x00000000}}, // 2^64
  .expected_return_code = 0,
  .test_name = "chain carry from bits[1] to bits[2]"
})

// Тесты переполнения

ADD_TEST_CASES(add_overflow_positive, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // INT_MAX
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 1,
  .test_name = "MAX_DECIMAL + 1 = OVERFLOW (1)"
})

ADD_TEST_CASES(add_overflow_negative, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}, // INT_MAX
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x80000000}}, // -1
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 2,
  .test_name = "MIN_DECIMAL + (-1) = OVERFLOW (2)"
})



Suite *add_suite_create(void) {
  Suite *s = suite_create("Additional");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_add_zero_is_positive);
  tcase_add_test(tc, test_add_zero);
  tcase_add_test(tc, test_add_positive);
  tcase_add_test(tc, test_add_negative);
  tcase_add_test(tc, test_add_mixed_negative);
  tcase_add_test(tc, test_add_mixed_positive);
  tcase_add_test(tc, test_add_big_numbers_without_overflow_0_to_1);
  tcase_add_test(tc, test_add_big_numbers_without_overflow_1_to_2);
  tcase_add_test(tc, test_add_overflow_positive);
  tcase_add_test(tc, test_add_overflow_negative);
  suite_add_tcase(s, tc);
  return s;
}
