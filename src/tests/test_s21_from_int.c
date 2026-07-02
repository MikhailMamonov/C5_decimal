#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"
#include "../convertors.h"

static void run_from_int_test(fromIntParams *params) {
  s21_decimal dst = {0};

  int return_code = s21_from_int_to_decimal(params->src, &dst);

  ck_assert_int_eq(return_code, params->expected_return_code);
  
  char str_res[128] = {0};
  char str_exp[128] = {0};

  decimal_to_string(dst, str_res);
  decimal_to_string(params->expected_result, str_exp);

  ck_assert_msg(compare_decimal(dst, params->expected_result),
      " FAIL [%s]:\n result_code = '%d' expected_return_code = '%d'\n result = '%s'\n expected = '%s'.",
      params->test_name, return_code, params->expected_return_code, str_res, str_exp);

  printf("[PASS] %s\n", params->test_name);
}

FROM_INT_TEST_CASES(from_int_basic_positive, {
  .src = 15,
  .expected_result = {{0x0000000F, 0x00000000, 0x00000000, 0x00000000}}, // 15, sign = 0, scale = 0
  .expected_return_code = 0,
  .test_name = "Basic positive int: 15"
})

FROM_INT_TEST_CASES(from_int_basic_negative, {
  .src = -15,
  .expected_result = {{0x0000000F, 0x00000000, 0x00000000, 0x80000000}}, // 15, sign = 1 (0x80000000), scale = 0
  .expected_return_code = 0,
  .test_name = "Basic negative int: -15"
})

FROM_INT_TEST_CASES(from_int_minus_one, {
  .src = -1,
  // Проверяет, что дополнительный код 0xFFFFFFFF успешно превратился в чистый модуль 1 со знаком минус
  .expected_result = {{0x00000001, 0x00000000, 0x00000000, 0x80000000}}, 
  .expected_return_code = 0,
  .test_name = "Negative one int: -1"
})

FROM_INT_TEST_CASES(from_int_pure_zero, {
  .src = 0,
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0, sign = 0, scale = 0
  .expected_return_code = 0,
  .test_name = "Pure zero int: 0"
})

FROM_INT_TEST_CASES(from_int_max_limit, {
  .src = 2147483647, // INT_MAX
  .expected_result = {{0x7FFFFFFF, 0x00000000, 0x00000000, 0x00000000}}, 
  .expected_return_code = 0,
  .test_name = "Max int limit: 2147483647"
})

FROM_INT_TEST_CASES(from_int_min_limit, {
  .src = -2147483648, // INT_MIN
  // Проверяет, что 0x80000000 на беззнаковом уровне устоял и записался как модуль 2147483648 со знаком минус
  .expected_result = {{0x80000000, 0x00000000, 0x00000000, 0x80000000}}, 
  .expected_return_code = 0,
  .test_name = "Min int limit: -2147483648"
})



Suite *from_int_suite_create(void) {
  Suite *s = suite_create("From int");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_from_int_basic_positive);
  tcase_add_test(tc, test_from_int_basic_negative);
  tcase_add_test(tc, test_from_int_minus_one);
  tcase_add_test(tc, test_from_int_pure_zero);
  tcase_add_test(tc, test_from_int_max_limit);
  tcase_add_test(tc, test_from_int_min_limit);

  suite_add_tcase(s, tc);
  return s;
}