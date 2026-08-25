#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../convertors.h"
#include "../s21_decimal.h"
#include "test_s21_common.h"

static void run_to_float_test(toFloatParams *params) {
  float dst = 0;

  int return_code = s21_from_decimal_to_float(params->src, &dst);

  ck_assert_int_eq(return_code, params->expected_return_code);

  char str_res[128] = {0};
  char str_exp[128] = {0};

  sprintf(str_res, "%f", dst);
  sprintf(str_exp, "%f", params->expected_result);

  ck_assert_float_eq(dst, params->expected_result);

  /* ck_assert_msg(ck_assert_float_eq(dst, params->expected_result),
       " FAIL [%s]:\n result_code = '%d' expected_return_code = '%d'\n result =
     '%s'\n expected = '%s'.", params->test_name, return_code,
     params->expected_return_code, str_res, str_exp);*/

  printf("[PASS] %s\n", params->test_name);
}

TO_FLOAT_TEST_CASES(to_float_basic_positive,
                    {.src = {{0x0000000F, 0x00000000, 0x00000000,
                              0x00000000}},  // 15
                     .expected_result = 15.0f,
                     .expected_return_code = 0,
                     .test_name = "Basic positive float: 15"})

TO_FLOAT_TEST_CASES(to_float_basic_negative,
                    {.src = {{0x0000000F, 0x00000000, 0x00000000,
                              0x80000000}},  // -15
                     .expected_result = -15.0f,
                     .expected_return_code = 0,
                     .test_name = "Basic negative float: -15"})

TO_FLOAT_TEST_CASES(to_float_with_scale,
                    {.src = {{0x0012D687, 0x00000000, 0x00000000,
                              0x00070000}},  // мантисса 1234567, scale 7
                     .expected_result = 0.1234567f,
                     .expected_return_code = 0,
                     .test_name = "Fractional decimal to float: 0.1234567"})

TO_FLOAT_TEST_CASES(to_float_pure_zero,
                    {.src = {{0x00000000, 0x00000000, 0x00000000,
                              0x00000000}},  // 0
                     .expected_result = 0.0f,
                     .expected_return_code = 0,
                     .test_name = "Pure zero decimal to float: 0.0"})

TO_FLOAT_TEST_CASES(to_float_minus_zero,
                    {.src = {{0x00000000, 0x00000000, 0x00000000,
                              0x80000000}},  // -0
                     // Проверяет, что аппаратное сохранение знака минус-ноль
                     // отработает корректно
                     .expected_result = -0.0f,
                     .expected_return_code = 0,
                     .test_name = "Minus zero decimal to float: -0.0"})

TO_FLOAT_TEST_CASES(to_float_max_decimal,
                    {.src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                              0x00000000}},  // MAX_DECIMAL
                     // 79228162514264337593543950335.0 в формате float
                     // округлится до 7.922816e+28
                     .expected_result = 79228162514264337593543950336.0f,
                     .expected_return_code = 0,
                     .test_name = "Max decimal to float"})

TO_FLOAT_TEST_CASES(to_float_one,
                    {.src = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},
                     .expected_result = 1.0f,
                     .expected_return_code = 0,
                     .test_name = "Decimal 1 to float 1.0"})

TO_FLOAT_TEST_CASES(to_float_minus_one,
                    {.src = {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},
                     .expected_result = -1.0f,
                     .expected_return_code = 0,
                     .test_name = "Decimal -1 to float -1.0"})

TO_FLOAT_TEST_CASES(to_float_max_scale,
                    {.src = {{0x00000001, 0x00000000, 0x00000000,
                              0x001C0000}},  // 1e-28
                     .expected_result = 1e-28f,
                     .expected_return_code = 0,
                     .test_name = "Min decimal value 1e-28 to float"})

TO_FLOAT_TEST_CASES(
    to_float_max_mantissa_max_scale,
    {.src = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
              0x001C0000}},  // MAX_DECIMAL with scale 28
     .expected_result = 7.9228162514264337593543950335f,
     .expected_return_code = 0,
     .test_name = "MAX_DECIMAL with scale 28 to float (avoids inf)"})

TO_FLOAT_TEST_CASES(to_float_bit_alternation,
                    {.src = {{0x55555555, 0x55555555, 0x55555555, 0x00000000}},
                     .expected_result = 26409388291815859676122185728.0f,
                     .expected_return_code = 0,
                     .test_name = "Alternating bits pattern to float"})

TO_FLOAT_TEST_CASES(to_float_high_bits_only,
                    {.src = {{0x00000000, 0x00000000, 0x00000001,
                              0x00000000}},  // 2^64
                     .expected_result = 18446744073709551616.0f,
                     .expected_return_code = 0,
                     .test_name = "Only bits[2] populated (2^64) to float"})

TO_FLOAT_TEST_CASES(to_float_half_round_up,
                    {.src = {{0x001E8481, 0x00000000, 0x00000000,
                              0x00060000}},  // 2000001 with scale 6 -> 2.000001
                     .expected_result = 2.000001f,
                     .expected_return_code = 0,
                     .test_name = "Float rounding check up"})

TO_FLOAT_TEST_CASES(to_float_half_round_down,
                    {.src = {{0x001E847F, 0x00000000, 0x00000000,
                              0x00060000}},  // 1999999 with scale 6 -> 1.999999
                     .expected_result = 1.999999f,
                     .expected_return_code = 0,
                     .test_name = "Float rounding check down"})

TO_FLOAT_TEST_CASES(
    to_float_large_with_scale,
    {.src = {{0xFFFFFFFF, 0x000000FF, 0x00000000,
              0x00050000}},  // large 64-bit mantissa with scale 5
     .expected_result = 10995116.0f,
     .expected_return_code = 0,
     .test_name = "Large 64-bit mantissa with scale 5 to float"})

TO_FLOAT_TEST_CASES(
    to_float_small_negative_fraction,
    {.src = {{0x00003039, 0x00000000, 0x00000000,
              0x800B0000}},  // -12345 with scale 11 -> -0.00000012345
     .expected_result = -0.00000012345f,
     .expected_return_code = 0,
     .test_name = "Small negative fraction to float"})

Suite *to_float_suite_create(void) {
  Suite *s = suite_create("To float");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_to_float_basic_positive);
  tcase_add_test(tc, test_to_float_basic_negative);
  tcase_add_test(tc, test_to_float_with_scale);
  tcase_add_test(tc, test_to_float_pure_zero);
  tcase_add_test(tc, test_to_float_minus_zero);
  tcase_add_test(tc, test_to_float_max_decimal);
  tcase_add_test(tc, test_to_float_one);
  tcase_add_test(tc, test_to_float_minus_one);
  tcase_add_test(tc, test_to_float_max_scale);
  tcase_add_test(tc, test_to_float_max_mantissa_max_scale);
  tcase_add_test(tc, test_to_float_bit_alternation);
  tcase_add_test(tc, test_to_float_high_bits_only);
  tcase_add_test(tc, test_to_float_half_round_up);
  tcase_add_test(tc, test_to_float_half_round_down);
  tcase_add_test(tc, test_to_float_large_with_scale);
  tcase_add_test(tc, test_to_float_small_negative_fraction);

  suite_add_tcase(s, tc);
  return s;
}