#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"
#include "../convertors.h"

static void run_to_int_test(toIntParams *params) {
  int dst = 0;

  int return_code = s21_from_decimal_to_int(params->src, &dst);

  ck_assert_int_eq(return_code, params->expected_return_code);
  
  char str_res[128] = {0};
  char str_exp[128] = {0};

  sprintf(str_res, "%d", dst);
  sprintf(str_exp, "%d", params->expected_result);

  ck_assert_msg(dst == params->expected_result,
      " FAIL [%s]:\n result_code = '%d' expected_return_code = '%d'\n result = '%s'\n expected = '%s'.",
      params->test_name, return_code, params->expected_return_code, str_res, str_exp);

  printf("[PASS] %s\n", params->test_name);
}

TO_INT_TEST_CASES(to_int_basic_positive, {
  .src = {{0x0000000F, 0x00000000, 0x00000000, 0x00000000}}, // 15
  .expected_result = 15,
  .expected_return_code = 0,
  .test_name = "Basic positive int: 15"
})

TO_INT_TEST_CASES(to_int_basic_negative, {
  .src = {{0x0000000F, 0x00000000, 0x00000000, 0x80000000}}, // -15
  .expected_result = -15,
  .expected_return_code = 0,
  .test_name = "Basic negative int: -15"
})

TO_INT_TEST_CASES(to_int_with_truncate, {
  .src = {{0x0000007B, 0x00000000, 0x00000000, 0x00020000}}, // 1.23 (scale 2)
  // Дробная часть .23 должна просто отсечься, в результат пойдет чистая единица
  .expected_result = 1,
  .expected_return_code = 0,
  .test_name = "Truncate fractional part: 1.23 to 1"
})

TO_INT_TEST_CASES(to_int_max_limit, {
  .src = {{0x7FFFFFFF, 0x00000000, 0x00000000, 0x00000000}}, // INT_MAX
  .expected_result = 2147483647,
  .expected_return_code = 0,
  .test_name = "Max int limit: 2147483647"
})

TO_INT_TEST_CASES(to_int_min_limit, {
  .src = {{0x80000000, 0x00000000, 0x00000000, 0x80000000}}, // INT_MIN (-2147483648)
  // Проверяет, что модуль 0x80000000 со знаком минус корректно собрался в знаковый int
  .expected_result = -2147483648,
  .expected_return_code = 0,
  .test_name = "Min int limit: -2147483648"
})

TO_INT_TEST_CASES(to_int_overflow_error, {
  .src = {{0x80000000, 0x00000000, 0x00000000, 0x00000000}}, // +2147483648
  // Это число превышает INT_MAX на единицу, должно возвращать ошибку конвертации (1)
  .expected_result = 0,
  .expected_return_code = 1, // ERROR
  .test_name = "Positive overflow error: 2147483648"
})

TO_INT_TEST_CASES(to_int_huge_overflow_error, {
  .src = {{0x00000000, 0x00000001, 0x00000000, 0x00000000}}, // 2^32 (единица в bits[1])
  // Число занимает больше одного инта мантиссы, гарантированная ошибка конвертации (1)
  .expected_result = 0,
  .expected_return_code = 1, // ERROR
  .test_name = "Huge overflow error (multiple bits blocks populated)"
})




Suite *to_int_suite_create(void) {
  Suite *s = suite_create("To int");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_to_int_basic_positive);
    tcase_add_test(tc, test_to_int_basic_negative);
    tcase_add_test(tc, test_to_int_with_truncate);
    tcase_add_test(tc, test_to_int_max_limit);
    tcase_add_test(tc, test_to_int_min_limit);
    tcase_add_test(tc, test_to_int_overflow_error);
    tcase_add_test(tc, test_to_int_huge_overflow_error);

  suite_add_tcase(s, tc);
  return s;
}