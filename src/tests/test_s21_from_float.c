#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"
#include "../convertors.h"

static void run_from_float_test(fromFloatParams *params) {
  s21_decimal dst = {0};

  int return_code = s21_from_float_to_decimal(params->src, &dst);

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

FROM_FLOAT_TEST_CASES(from_float_basic_positive, {
  .src = 15.0f,
  // 15.0 в формате %.6e — это 1.500000e+01. 
  // Мантисса: 1500000 (0x0016E360), масштаб: 6 - 1 = 5.
  .expected_result = {{0x0016E360, 0x00000000, 0x00000000, 0x00050000}}, 
  .expected_return_code = 0,
  .test_name = "Basic positive float: 15.0"
})

FROM_FLOAT_TEST_CASES(from_float_basic_negative, {
  .src = -15.0f,
  // Зеркальный тест для проверки знака минус (0x80050000)
  .expected_result = {{0x0016E360, 0x00000000, 0x00000000, 0x80050000}}, 
  .expected_return_code = 0,
  .test_name = "Basic negative float: -15.0"
})

FROM_FLOAT_TEST_CASES(from_float_pure_zero, {
  .src = 0.0f,
  // 0.0 в формате %.6e — это 0.000000e+00. 
  // Мантисса: 0, масштаб: 6 - 0 = 6.
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00060000}}, 
  .expected_return_code = 0,
  .test_name = "Pure zero float: 0.0"
})

FROM_FLOAT_TEST_CASES(from_float_small_fraction, {
  .src = 0.1234567f,
  // 0.1234567 в формате %.6e — это 1.234567e-01. 
  // Мантисса: 1234567 (0x0012D687), масштаб: 6 - (-1) = 7.
  .expected_result = {{0x0012D687, 0x00000000, 0x00000000, 0x00070000}}, 
  .expected_return_code = 0,
  .test_name = "Small fraction float: 0.1234567"
})

FROM_FLOAT_TEST_CASES(from_float_large_number, {
  .src = 123456700.0f,
  // 123456700.0 в формате %.6e — это 1.234567e+08. 
  // Мантисса: 1234567. Масштаб: 6 - 8 = -2. 
  // Ваш аварийный цикл нормализации умножит мантиссу на 100 -> 123456700 (0x075BCD44), масштаб станет 0.
  .expected_result = {{0x075BCCBC, 0x00000000, 0x00000000, 0x00000000}}, 
  .expected_return_code = 0,
  .test_name = "Large float with negative scale normalization"
})

FROM_FLOAT_TEST_CASES(from_float_too_small_limit, {
  .src = 1e-30f,
  // Число меньше чем 1e-28, должно возвращать ошибку конвертации по ТЗ Школы 21
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, 
  .expected_return_code = 1, // ERROR
  .test_name = "Too small float underflow limit error"
})

Suite *from_float_suite_create(void) {
  Suite *s = suite_create("From float");
  TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_from_float_basic_positive);
    tcase_add_test(tc, test_from_float_basic_negative);
    tcase_add_test(tc, test_from_float_pure_zero);
    tcase_add_test(tc, test_from_float_small_fraction);
    tcase_add_test(tc, test_from_float_large_number);
    tcase_add_test(tc, test_from_float_too_small_limit);

  suite_add_tcase(s, tc);
  return s;
}