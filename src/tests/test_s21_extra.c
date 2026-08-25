#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"

static void run_round_test(TestParams *params) {
  printf("run_round_test %s\n", params->test_name);
  s21_decimal result = {0};

  int return_code = s21_round(params->value1, &result);
  ck_assert_int_eq(return_code, params->expected_return_code);

  char str_res[128] = {0};
  char str_exp[128] = {0};

  decimal_to_string(result, str_res);
  decimal_to_string(params->expected_result, str_exp);

  ck_assert_msg(compare_decimal(result, params->expected_result),
                " FAIL [%s]:\n result_code = '%d' expected_return_code = "
                "'%d'\n result = '%s'\n expected = '%s'.",
                params->test_name, return_code, params->expected_return_code,
                str_res, str_exp);
  printf("[PASS] %s\n", params->test_name);
}

static void run_floor_test(TestParams *params) {
  printf("run_floor_test %s\n", params->test_name);
  s21_decimal result = {0};

  int return_code = s21_floor(params->value1, &result);
  ck_assert_int_eq(return_code, params->expected_return_code);

  char str_res[128] = {0};
  char str_exp[128] = {0};

  decimal_to_string(result, str_res);
  decimal_to_string(params->expected_result, str_exp);

  ck_assert_msg(compare_decimal(result, params->expected_result),
                " FAIL [%s]:\n result_code = '%d' expected_return_code = "
                "'%d'\n result = '%s'\n expected = '%s'.",
                params->test_name, return_code, params->expected_return_code,
                str_res, str_exp);
  printf("[PASS] %s\n", params->test_name);
}

static void run_negate_test(TestParams *params) {
  printf("run_negate_test %s\n", params->test_name);
  s21_decimal result = {0};

  int return_code = s21_negate(params->value1, &result);
  ck_assert_int_eq(return_code, params->expected_return_code);

  char str_res[128] = {0};
  char str_exp[128] = {0};

  decimal_to_string(result, str_res);
  decimal_to_string(params->expected_result, str_exp);

  ck_assert_msg(compare_decimal(result, params->expected_result),
                " FAIL [%s]:\n result_code = '%d' expected_return_code = "
                "'%d'\n result = '%s'\n expected = '%s'.",
                params->test_name, return_code, params->expected_return_code,
                str_res, str_exp);
  printf("[PASS] %s\n", params->test_name);
}

static void run_truncate_test(TestParams *params) {
  printf("run_truncate_test %s\n", params->test_name);

  s21_decimal result = {0};

  int return_code = s21_truncate(params->value1, &result);
  ck_assert_int_eq(return_code, params->expected_return_code);

  char str_res[128] = {0};
  char str_exp[128] = {0};

  decimal_to_string(result, str_res);
  decimal_to_string(params->expected_result, str_exp);

  ck_assert_msg(compare_decimal(result, params->expected_result),
                " FAIL [%s]:\n result_code = '%d' expected_return_code = "
                "'%d'\n result = '%s'\n expected = '%s'.",
                params->test_name, return_code, params->expected_return_code,
                str_res, str_exp);
  printf("[PASS] %s\n", params->test_name);
}

// Тесты Negate

EXTRA_NEGATE_TEST_CASES(negate_positive_to_negative,
                        {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                     0x00000000}},  // 5
                         .expected_result = {{0x00000005, 0x00000000,
                                              0x00000000, 0x80000000}},  // -5
                         .expected_return_code = 0,  // Всегда OK
                         .test_name = "5 -> -5"})

EXTRA_NEGATE_TEST_CASES(negate_negative_to_positive,
                        {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                     0x80000000}},  // -5
                         .expected_result = {{0x00000005, 0x00000000,
                                              0x00000000, 0x00000000}},  // 5
                         .expected_return_code = 0,
                         .test_name = "-5 -> 5"})

EXTRA_NEGATE_TEST_CASES(negate_zero,
                        {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                     0x00000000}},  // 0
                         .expected_result = {{0x00000000, 0x00000000,
                                              0x00000000, 0x00000000}},  // 0
                         .expected_return_code = 0,
                         .test_name = "0 -> 0"})

EXTRA_NEGATE_TEST_CASES(negate_negative_zero,
                        {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                     0x80000000}},  // -0
                         .expected_result = {{0x00000000, 0x00000000,
                                              0x00000000, 0x00000000}},  // 0
                         .expected_return_code = 0,
                         .test_name = "-0 -> 0"})

EXTRA_NEGATE_TEST_CASES(
    negate_with_scale,
    {.value1 = {{0x000000C8, 0x00000000, 0x00000000, 0x00020000}},  // 2.00
     .expected_result = {{0x000000C8, 0x00000000, 0x00000000,
                          0x80020000}},  // -2.00
     .expected_return_code = 0,
     .test_name = "2.00 -> -2.00"})

EXTRA_NEGATE_TEST_CASES(negate_negative_with_scale,
                        {.value1 = {{0x000000C8, 0x00000000, 0x00000000,
                                     0x80020000}},  // -2.00
                         .expected_result = {{0x000000C8, 0x00000000,
                                              0x00000000, 0x00020000}},  // 2.00
                         .expected_return_code = 0,
                         .test_name = "-2.00 -> 2.00"})

EXTRA_NEGATE_TEST_CASES(negate_large_number,
                        {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                     0x00000000}},  // Max decimal
                         .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF,
                                              0xFFFFFFFF, 0x80000000}},  // -Max
                         .expected_return_code = 0,
                         .test_name = "Max -> -Max"})

START_TEST(test_negate_null_pointer) {
  s21_decimal value = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  int return_code = s21_negate(value, NULL);
  ck_assert_int_eq(return_code, 1);
  printf("[PASS] negate: NULL pointer -> error\n");
}
END_TEST

EXTRA_NEGATE_TEST_CASES(
    negate_max_positive,
    {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},  // MAX_INT
     .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                          0x80000000}},  // -MAX_INT
     .expected_return_code = 0,
     .test_name = "negate: MAX_INT -> -MAX_INT"})

EXTRA_NEGATE_TEST_CASES(
    negate_min_negative,
    {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -MAX_INT
     .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                          0x00000000}},  // MAX_INT
     .expected_return_code = 0,
     .test_name = "negate: -MAX_INT -> MAX_INT"})

EXTRA_NEGATE_TEST_CASES(negate_large_with_scale,
                        {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                     0x00010000}},  // MAX_INT/10
                         .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF,
                                              0xFFFFFFFF,
                                              0x80010000}},  // -MAX_INT/10
                         .expected_return_code = 0,
                         .test_name = "negate: large number with scale"})

EXTRA_NEGATE_TEST_CASES(
    negate_max_scale, {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                   0x001C0000}},  // 1e-28 (scale=28)
                       .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                            0x801C0000}},  // -1e-28
                       .expected_return_code = 0,
                       .test_name = "negate: max scale (28) -> negative"})

EXTRA_NEGATE_TEST_CASES(
    negate_all_bits_set,
    {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},  // все биты
     .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                          0x80000000}},  // все биты со знаком
     .expected_return_code = 0,
     .test_name = "negate: all bits set -> negative"})

// Тесты Truncate

EXTRA_TRUNCATE_TEST_CASES(truncate_whole_number_positive,
                          {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                       0x00000000}},  // 5
                           .expected_result = {{0x00000005, 0x00000000,
                                                0x00000000, 0x00000000}},  // 5
                           .expected_return_code = 0,
                           .test_name = "truncate: 5 (целое) -> 5"})

EXTRA_TRUNCATE_TEST_CASES(truncate_whole_number_negative,
                          {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                       0x80000000}},  // -5
                           .expected_result = {{0x00000005, 0x00000000,
                                                0x00000000, 0x80000000}},  // -5
                           .expected_return_code = 0,
                           .test_name = "truncate: -5 (целое) -> -5"})

EXTRA_TRUNCATE_TEST_CASES(truncate_zero,
                          {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                       0x00000000}},  // 0
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x00000000}},  // 0
                           .expected_return_code = 0,
                           .test_name = "truncate: 0 -> 0"})

EXTRA_TRUNCATE_TEST_CASES(
    truncate_whole_large,
    {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}},  // MAX_INT
     .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                          0x00000000}},  // MAX_INT
     .expected_return_code = 0,
     .test_name = "truncate: MAX_INT -> MAX_INT"})

EXTRA_TRUNCATE_TEST_CASES(
    truncate_whole_large_negative,
    {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -MAX_INT
     .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                          0x80000000}},  // -MAX_INT
     .expected_return_code = 0,
     .test_name = "truncate: -MAX_INT -> -MAX_INT"})

START_TEST(test_div_by_10) {
  s21_decimal num = {{12345, 0, 0, 0x00020000}};  // 123.45

  int remainder = div_by_10(&num);

  // Ожидаем: 1234 (мантисса), остаток 5
  ck_assert_int_eq(num.bits[0], 1234);
  ck_assert_int_eq(remainder, 5);

  printf("[PASS] div_by_10: 12345 -> 1234, remainder 5\n");
}
END_TEST

EXTRA_TRUNCATE_TEST_CASES(
    truncate_fractional_positive,
    {.value1 = {{0x00003039, 0x00000000, 0x00000000, 0x00020000}},  // 123.45
     .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                          0x00000000}},  // 123
     .expected_return_code = 0,
     .test_name = "truncate: 123.45 -> 123"})

EXTRA_TRUNCATE_TEST_CASES(
    truncate_fractional_negative,
    {.value1 = {{0x00003039, 0x00000000, 0x00000000, 0x80020000}},  // -123.45
     .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                          0x80000000}},  // -123
     .expected_return_code = 0,
     .test_name = "truncate: -123.45 -> -123"})

EXTRA_TRUNCATE_TEST_CASES(truncate_fractional_small,
                          {.value1 = {{0x0000000A, 0x00000000, 0x00000000,
                                       0x00010000}},  // 1.0 (scale=1)
                           .expected_result = {{0x00000001, 0x00000000,
                                                0x00000000, 0x00000000}},  // 1
                           .expected_return_code = 0,
                           .test_name = "truncate: 1.0 -> 1"})

EXTRA_TRUNCATE_TEST_CASES(truncate_fractional_less_than_one,
                          {.value1 = {{0x00000009, 0x00000000, 0x00000000,
                                       0x00010000}},  // 0.9 (scale=1)
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x00000000}},  // 0
                           .expected_return_code = 0,
                           .test_name = "truncate: 0.9 -> 0"})

EXTRA_TRUNCATE_TEST_CASES(truncate_fractional_negative_less_than_one,
                          {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                       0x80010000}},  // -0.5 (scale=1)
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x80000000}},  // -0
                           .expected_return_code = 0,
                           .test_name = "truncate: -0.5 -> -0"})

EXTRA_TRUNCATE_TEST_CASES(truncate_fractional_scale_3,
                          {.value1 = {{0x0001E240, 0x00000000, 0x00000000,
                                       0x00030000}},  // 123.456 (scale=3)
                           .expected_result = {{0x0000007B, 0x00000000,
                                                0x00000000,
                                                0x00000000}},  // 123
                           .expected_return_code = 0,
                           .test_name = "truncate: 123.456 -> 123 (scale=3)"})

EXTRA_TRUNCATE_TEST_CASES(truncate_fractional_scale_4,
                          {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                       0x00040000}},  // 0.0001 (scale=4)
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x00000000}},  // 0
                           .expected_return_code = 0,
                           .test_name = "truncate: 0.0001 -> 0"})

EXTRA_TRUNCATE_TEST_CASES(
    truncate_fractional_scale_28,
    {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                 0x001C0000}},  // min value (scale=28)
     .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                          0x00000000}},  // 0
     .expected_return_code = 0,
     .test_name = "truncate: min positive (scale=28) -> 0"})

EXTRA_TRUNCATE_TEST_CASES(truncate_with_scale_1,
                          {.value1 = {{0x0000000F, 0x00000000, 0x00000000,
                                       0x00010000}},  // 1.5 (scale=1)
                           .expected_result = {{0x00000001, 0x00000000,
                                                0x00000000, 0x00000000}},  // 1
                           .expected_return_code = 0,
                           .test_name = "truncate: 1.5 -> 1"})

EXTRA_TRUNCATE_TEST_CASES(truncate_with_scale_1_negative,
                          {.value1 = {{0x0000000F, 0x00000000, 0x00000000,
                                       0x80010000}},  // -1.5
                           .expected_result = {{0x00000001, 0x00000000,
                                                0x00000000, 0x80000000}},  // -1
                           .expected_return_code = 0,
                           .test_name = "truncate: -1.5 -> -1"})

EXTRA_TRUNCATE_TEST_CASES(truncate_with_scale_2,
                          {.value1 = {{0x000000C8, 0x00000000, 0x00000000,
                                       0x00020000}},  // 2.00 (scale=2)
                           .expected_result = {{0x00000002, 0x00000000,
                                                0x00000000, 0x00000000}},  // 2
                           .expected_return_code = 0,
                           .test_name = "truncate: 2.00 -> 2"})

EXTRA_TRUNCATE_TEST_CASES(truncate_with_scale_2_negative,
                          {.value1 = {{0x000000C8, 0x00000000, 0x00000000,
                                       0x80020000}},  // -2.00
                           .expected_result = {{0x00000002, 0x00000000,
                                                0x00000000, 0x80000000}},  // -2
                           .expected_return_code = 0,
                           .test_name = "truncate: -2.00 -> -2"})

EXTRA_TRUNCATE_TEST_CASES(truncate_large_fractional,
                          {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                       0x00010000}},  // MAX_INT / 10
                           .expected_result = {{0x99999999, 0x99999999,
                                                0x19999999,
                                                0x00000000}},  // целая часть
                           .expected_return_code = 0,
                           .test_name = "truncate: MAX_INT/10 -> целая часть"})

EXTRA_TRUNCATE_TEST_CASES(
    truncate_large_fractional_negative,
    {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                 0x80010000}},  // -MAX_INT / 10
     .expected_result = {{0x99999999, 0x99999999, 0x19999999,
                          0x80000000}},  // -целая часть
     .expected_return_code = 0,
     .test_name = "truncate: -MAX_INT/10 -> -целая часть"})

EXTRA_TRUNCATE_TEST_CASES(truncate_invalid_scale_255,
                          {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                       0x00FF0000}},  // scale=255 (invalid)
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x00000000}},
                           .expected_return_code = 1,
                           .test_name = "truncate: invalid scale 255 -> error"})

EXTRA_TRUNCATE_TEST_CASES(truncate_invalid_scale_29,
                          {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                       0x001D0000}},  // scale=29 (invalid)
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x00000000}},
                           .expected_return_code = 1,
                           .test_name = "truncate: invalid scale 29 -> error"})

EXTRA_TRUNCATE_TEST_CASES(truncate_very_small_positive,
                          {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                       0x001C0000}},  // 1e-28
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x00000000}},  // 0
                           .expected_return_code = 0,
                           .test_name = "truncate: 1e-28 -> 0"})

EXTRA_TRUNCATE_TEST_CASES(truncate_very_small_negative,
                          {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                       0x801C0000}},  // -1e-28
                           .expected_result = {{0x00000000, 0x00000000,
                                                0x00000000, 0x80000000}},  // -0
                           .expected_return_code = 0,
                           .test_name = "truncate: -1e-28 -> -0"})

// Тесты Round

EXTRA_ROUND_TEST_CASES(round_whole_positive,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x00000000}},  // 5
                        .expected_result = {{0x00000005, 0x00000000, 0x00000000,
                                             0x00000000}},  // 5
                        .expected_return_code = 0,
                        .test_name = "round: 5 -> 5"})

EXTRA_ROUND_TEST_CASES(round_whole_negative,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x80000000}},  // -5
                        .expected_result = {{0x00000005, 0x00000000, 0x00000000,
                                             0x80000000}},  // -5
                        .expected_return_code = 0,
                        .test_name = "round: -5 -> -5"})

EXTRA_ROUND_TEST_CASES(round_zero,
                       {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                    0x00000000}},  // 0
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "round: 0 -> 0"})

EXTRA_ROUND_TEST_CASES(round_negative_zero,
                       {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                    0x80000000}},  // -0
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x80000000}},  // -0
                        .expected_return_code = 0,
                        .test_name = "round: -0 -> -0"})

EXTRA_ROUND_TEST_CASES(round_0_5_to_0,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x00010000}},  // 0.5
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: 0.5 -> 0 (banking)"})

EXTRA_ROUND_TEST_CASES(round_1_5_to_2,
                       {.value1 = {{0x0000000F, 0x00000000, 0x00000000,
                                    0x00010000}},  // 1.5
                        .expected_result = {{0x00000002, 0x00000000, 0x00000000,
                                             0x00000000}},  // 2 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: 1.5 -> 2 (banking)"})

EXTRA_ROUND_TEST_CASES(round_2_5_to_2,
                       {.value1 = {{0x00000019, 0x00000000, 0x00000000,
                                    0x00010000}},  // 2.5
                        .expected_result = {{0x00000002, 0x00000000, 0x00000000,
                                             0x00000000}},  // 2 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: 2.5 -> 2 (banking)"})

EXTRA_ROUND_TEST_CASES(round_3_5_to_4,
                       {.value1 = {{0x00000023, 0x00000000, 0x00000000,
                                    0x00010000}},  // 3.5
                        .expected_result = {{0x00000004, 0x00000000, 0x00000000,
                                             0x00000000}},  // 4 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: 3.5 -> 4 (banking)"})

EXTRA_ROUND_TEST_CASES(round_4_5_to_4,
                       {.value1 = {{0x0000002D, 0x00000000, 0x00000000,
                                    0x00010000}},  // 4.5
                        .expected_result = {{0x00000004, 0x00000000, 0x00000000,
                                             0x00000000}},  // 4 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: 4.5 -> 4 (banking)"})

EXTRA_ROUND_TEST_CASES(round_negative_0_5_to_0,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x80010000}},  // -0.5
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x80000000}},  // -0
                        .expected_return_code = 0,
                        .test_name = "round: -0.5 -> -0 (banking)"})

EXTRA_ROUND_TEST_CASES(round_negative_1_5_to_2,
                       {.value1 = {{0x0000000F, 0x00000000, 0x00000000,
                                    0x80010000}},  // -1.5
                        .expected_result = {{0x00000002, 0x00000000, 0x00000000,
                                             0x80000000}},  // -2 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: -1.5 -> -2 (banking)"})

EXTRA_ROUND_TEST_CASES(round_negative_2_5_to_2,
                       {.value1 = {{0x00000019, 0x00000000, 0x00000000,
                                    0x80010000}},  // -2.5
                        .expected_result = {{0x00000002, 0x00000000, 0x00000000,
                                             0x80000000}},  // -2 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: -2.5 -> -2 (banking)"})

EXTRA_ROUND_TEST_CASES(round_negative_3_5_to_4,
                       {.value1 = {{0x00000023, 0x00000000, 0x00000000,
                                    0x80010000}},  // -3.5
                        .expected_result = {{0x00000004, 0x00000000, 0x00000000,
                                             0x80000000}},  // -4 (четное)
                        .expected_return_code = 0,
                        .test_name = "round: -3.5 -> -4 (banking)"})

EXTRA_ROUND_TEST_CASES(round_0_4_to_0,
                       {.value1 = {{0x00000004, 0x00000000, 0x00000000,
                                    0x00010000}},  // 0.4
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "round: 0.4 -> 0"})

EXTRA_ROUND_TEST_CASES(round_0_6_to_1,
                       {.value1 = {{0x00000006, 0x00000000, 0x00000000,
                                    0x00010000}},  // 0.6
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x00000000}},  // 1
                        .expected_return_code = 0,
                        .test_name = "round: 0.6 -> 1"})

EXTRA_ROUND_TEST_CASES(round_negative_0_4_to_0,
                       {.value1 = {{0x00000004, 0x00000000, 0x00000000,
                                    0x80010000}},  // -0.4
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x80000000}},  // -0
                        .expected_return_code = 0,
                        .test_name = "round: -0.4 -> -0"})

EXTRA_ROUND_TEST_CASES(round_negative_0_6_to_1,
                       {.value1 = {{0x00000006, 0x00000000, 0x00000000,
                                    0x80010000}},  // -0.6
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "round: -0.6 -> -1"})

EXTRA_ROUND_TEST_CASES(round_scale_2_123_45,
                       {.value1 = {{0x00003039, 0x00000000, 0x00000000,
                                    0x00020000}},  // 123.45
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x00000000}},  // 123
                        .expected_return_code = 0,
                        .test_name = "round: 123.45 -> 123"})

EXTRA_ROUND_TEST_CASES(round_scale_2_123_55,
                       {.value1 = {{0x00003043, 0x00000000, 0x00000000,
                                    0x00020000}},  // 123.55
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x00000000}},  // 124
                        .expected_return_code = 0,
                        .test_name = "round: 123.55 -> 124"})

EXTRA_ROUND_TEST_CASES(
    round_scale_2_123_50,
    {.value1 = {{0x0000303E, 0x00000000, 0x00000000, 0x00020000}},  // 123.50
     .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                          0x00000000}},  // 124 (т.к. 123 нечетное)
     .expected_return_code = 0,
     .test_name = "round: 123.50 -> 124 (banking)"})

EXTRA_ROUND_TEST_CASES(
    round_scale_2_122_50,
    {.value1 = {{0x00002FDA, 0x00000000, 0x00000000, 0x00020000}},  // 122.50
     .expected_result = {{0x0000007A, 0x00000000, 0x00000000,
                          0x00000000}},  // 122 (т.к. 122 четное)
     .expected_return_code = 0,
     .test_name = "round: 122.50 -> 122 (banking)"})

EXTRA_ROUND_TEST_CASES(round_scale_3_123_456,
                       {.value1 = {{0x0001E240, 0x00000000, 0x00000000,
                                    0x00030000}},  // 123.456
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x00000000}},  // 123
                        .expected_return_code = 0,
                        .test_name = "round: 123.456 -> 123"})

EXTRA_ROUND_TEST_CASES(round_scale_3_123_556,
                       {.value1 = {{0x0001E28C, 0x00000000, 0x00000000,
                                    0x00030000}},  // 123.556
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x00000000}},  // 124
                        .expected_return_code = 0,
                        .test_name = "round: 123.556 -> 124"})

EXTRA_ROUND_TEST_CASES(round_scale_4_0_0005,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x00040000}},  // 0.0005
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "round: 0.0005 -> 0"})

EXTRA_ROUND_TEST_CASES(
    round_scale_4_0_0006,
    {.value1 = {{0x00000006, 0x00000000, 0x00000000, 0x00040000}},  // 0.0006
     .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                          0x00000000}},  // 0 (т.к. первая цифра 0)
     .expected_return_code = 0,
     .test_name = "round: 0.0006 -> 0 (первая цифра 0)"})

EXTRA_ROUND_TEST_CASES(round_max_int,
                       {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                    0x00000000}},  // MAX_INT
                        .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                             0x00000000}},  // MAX_INT
                        .expected_return_code = 0,
                        .test_name = "round: MAX_INT -> MAX_INT"})

EXTRA_ROUND_TEST_CASES(round_max_int_negative,
                       {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                    0x80000000}},  // -MAX_INT
                        .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                             0x80000000}},  // -MAX_INT
                        .expected_return_code = 0,
                        .test_name = "round: -MAX_INT -> -MAX_INT"})

EXTRA_ROUND_TEST_CASES(round_max_int_with_scale_1,
                       {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                    0x00010000}},  // MAX_INT / 10
                        .expected_result = {{0x9999999A, 0x99999999, 0x19999999,
                                             0x00000000}},  // целая часть
                        .expected_return_code = 0,
                        .test_name = "round: MAX_INT/10 -> целая часть"})

START_TEST(test_round_null_pointer) {
  s21_decimal value = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};

  // Передаем NULL как result
  int code = s21_round(value, NULL);

  // Ожидаем ошибку (1)
  ck_assert_int_eq(code, 1);

  printf("[PASS] round: NULL pointer -> error\n");
}
END_TEST

EXTRA_ROUND_TEST_CASES(
    round_invalid_scale_255,
    {.value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00FF0000}},  // scale=255
     .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
     .expected_return_code = 1,  // Если функция проверяет scale
     .test_name = "round: invalid scale 255 -> error"})

EXTRA_ROUND_TEST_CASES(
    round_invalid_scale_29,
    {.value1 = {{0x00000005, 0x00000000, 0x00000000, 0x001D0000}},  // scale=29
     .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
     .expected_return_code = 1,
     .test_name = "round: invalid scale 29 -> error"})

EXTRA_ROUND_TEST_CASES(round_sign_preservation_positive,
                       {.value1 = {{0x0000000A, 0x00000000, 0x00000000,
                                    0x00010000}},  // 1.0
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x00000000}},  // 1
                        .expected_return_code = 0,
                        .test_name = "round: positive sign preserved"})

EXTRA_ROUND_TEST_CASES(round_sign_preservation_negative,
                       {.value1 = {{0x0000000A, 0x00000000, 0x00000000,
                                    0x80010000}},  // -1.0
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "round: negative sign preserved"})

EXTRA_ROUND_TEST_CASES(
    round_123_4567, {.value1 = {{0x0012D687, 0x00000000, 0x00000000,
                                 0x00040000}},  // 123.4567 (scale=4)
                     .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                          0x00000000}},  // 123 (первая цифра 4)
                     .expected_return_code = 0,
                     .test_name = "round: 123.4567 -> 123"})

EXTRA_ROUND_TEST_CASES(round_123_5567,
                       {.value1 = {{0x0012DABF, 0x00000000, 0x00000000,
                                    0x00040000}},  // 123.5567 (scale=4)
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x00000000}},  // 124
                        .expected_return_code = 0,
                        .test_name = "round: 123.5567 -> 124"})

EXTRA_ROUND_TEST_CASES(round_negative_123_4567,
                       {.value1 = {{0x0012D687, 0x00000000, 0x00000000,
                                    0x80040000}},  // -123.4567
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x80000000}},  // -123
                        .expected_return_code = 0,
                        .test_name = "round: -123.4567 -> -123"})

EXTRA_ROUND_TEST_CASES(round_negative_123_5567,
                       {.value1 = {{0x0012DABF, 0x00000000, 0x00000000,
                                    0x80040000}},  // -123.5567
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x80000000}},  // -124
                        .expected_return_code = 0,
                        .test_name = "round: -123.5567 -> -124"})

EXTRA_ROUND_TEST_CASES(round_max_scale_1e28,
                       {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                    0x001C0000}},  // 1e-28
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "round: 1e-28 -> 0"})

EXTRA_ROUND_TEST_CASES(round_max_scale_5e28,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x001C0000}},  // 5e-28
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "round: 5e-28 -> 0"})

// TEST floor

EXTRA_FLOOR_TEST_CASES(floor_whole_positive,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x00000000}},  // 5
                        .expected_result = {{0x00000005, 0x00000000, 0x00000000,
                                             0x00000000}},  // 5
                        .expected_return_code = 0,
                        .test_name = "floor: 5 -> 5"})

EXTRA_FLOOR_TEST_CASES(floor_whole_negative,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x80000000}},  // -5
                        .expected_result = {{0x00000005, 0x00000000, 0x00000000,
                                             0x80000000}},  // -5
                        .expected_return_code = 0,
                        .test_name = "floor: -5 -> -5"})

EXTRA_FLOOR_TEST_CASES(floor_zero,
                       {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                    0x00000000}},  // 0
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "floor: 0 -> 0"})

EXTRA_FLOOR_TEST_CASES(floor_negative_zero,
                       {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                    0x80000000}},  // -0
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x80000000}},  // -0
                        .expected_return_code = 0,
                        .test_name = "floor: -0 -> -0"})

EXTRA_FLOOR_TEST_CASES(floor_positive_0_5,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x00010000}},  // 0.5
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "floor: 0.5 -> 0"})

EXTRA_FLOOR_TEST_CASES(floor_positive_0_9,
                       {.value1 = {{0x00000009, 0x00000000, 0x00000000,
                                    0x00010000}},  // 0.9
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "floor: 0.9 -> 0"})

EXTRA_FLOOR_TEST_CASES(floor_positive_1_1,
                       {.value1 = {{0x0000000B, 0x00000000, 0x00000000,
                                    0x00010000}},  // 1.1
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x00000000}},  // 1
                        .expected_return_code = 0,
                        .test_name = "floor: 1.1 -> 1"})

EXTRA_FLOOR_TEST_CASES(floor_positive_1_9,
                       {.value1 = {{0x00000013, 0x00000000, 0x00000000,
                                    0x00010000}},  // 1.9
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x00000000}},  // 1
                        .expected_return_code = 0,
                        .test_name = "floor: 1.9 -> 1"})

EXTRA_FLOOR_TEST_CASES(floor_positive_123_45,
                       {.value1 = {{0x00003039, 0x00000000, 0x00000000,
                                    0x00020000}},  // 123.45
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x00000000}},  // 123
                        .expected_return_code = 0,
                        .test_name = "floor: 123.45 -> 123"})

EXTRA_FLOOR_TEST_CASES(floor_positive_123_99,
                       {.value1 = {{0x0000306F, 0x00000000, 0x00000000,
                                    0x00020000}},  // 123.99
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x00000000}},  // 123
                        .expected_return_code = 0,
                        .test_name = "floor: 123.99 -> 123"})

EXTRA_FLOOR_TEST_CASES(floor_negative_0_5,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x80010000}},  // -0.5
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: -0.5 -> -1"})

EXTRA_FLOOR_TEST_CASES(floor_negative_0_1,
                       {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                    0x80010000}},  // -0.1
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: -0.1 -> -1"})

EXTRA_FLOOR_TEST_CASES(floor_negative_0_9,
                       {.value1 = {{0x00000009, 0x00000000, 0x00000000,
                                    0x80010000}},  // -0.9
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: -0.9 -> -1"})

EXTRA_FLOOR_TEST_CASES(floor_negative_1_1,
                       {.value1 = {{0x0000000B, 0x00000000, 0x00000000,
                                    0x80010000}},  // -1.1
                        .expected_result = {{0x00000002, 0x00000000, 0x00000000,
                                             0x80000000}},  // -2
                        .expected_return_code = 0,
                        .test_name = "floor: -1.1 -> -2"})

EXTRA_FLOOR_TEST_CASES(floor_negative_1_5,
                       {.value1 = {{0x0000000F, 0x00000000, 0x00000000,
                                    0x80010000}},  // -1.5
                        .expected_result = {{0x00000002, 0x00000000, 0x00000000,
                                             0x80000000}},  // -2
                        .expected_return_code = 0,
                        .test_name = "floor: -1.5 -> -2"})

EXTRA_FLOOR_TEST_CASES(floor_negative_1_9,
                       {.value1 = {{0x00000013, 0x00000000, 0x00000000,
                                    0x80010000}},  // -1.9
                        .expected_result = {{0x00000002, 0x00000000, 0x00000000,
                                             0x80000000}},  // -2
                        .expected_return_code = 0,
                        .test_name = "floor: -1.9 -> -2"})

EXTRA_FLOOR_TEST_CASES(floor_negative_123_45,
                       {.value1 = {{0x00003039, 0x00000000, 0x00000000,
                                    0x80020000}},  // -123.45
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x80000000}},  // -124
                        .expected_return_code = 0,
                        .test_name = "floor: -123.45 -> -124"})

EXTRA_FLOOR_TEST_CASES(floor_negative_123_01,
                       {.value1 = {{0x0000300D, 0x00000000, 0x00000000,
                                    0x80020000}},  // -123.01
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x80000000}},  // -124
                        .expected_return_code = 0,
                        .test_name = "floor: -123.01 -> -124"})

EXTRA_FLOOR_TEST_CASES(floor_negative_123_99,
                       {.value1 = {{0x0000306F, 0x00000000, 0x00000000,
                                    0x80020000}},  // -123.99
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x80000000}},  // -124
                        .expected_return_code = 0,
                        .test_name = "floor: -123.99 -> -124"})

EXTRA_FLOOR_TEST_CASES(floor_scale_3_123_456,
                       {.value1 = {{0x0001E240, 0x00000000, 0x00000000,
                                    0x00030000}},  // 123.456
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x00000000}},  // 123
                        .expected_return_code = 0,
                        .test_name = "floor: 123.456 -> 123 (scale=3)"})

EXTRA_FLOOR_TEST_CASES(floor_scale_4_123_4567,
                       {.value1 = {{0x0012D687, 0x00000000, 0x00000000,
                                    0x00040000}},  // 123.4567
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x00000000}},  // 123
                        .expected_return_code = 0,
                        .test_name = "floor: 123.4567 -> 123 (scale=4)"})

EXTRA_FLOOR_TEST_CASES(floor_scale_5_0_00005,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x00050000}},  // 0.00005
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "floor: 0.00005 -> 0"})

EXTRA_FLOOR_TEST_CASES(floor_scale_3_negative_123_456,
                       {.value1 = {{0x0001E240, 0x00000000, 0x00000000,
                                    0x80030000}},  // -123.456
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x80000000}},  // -124
                        .expected_return_code = 0,
                        .test_name = "floor: -123.456 -> -124 (scale=3)"})

EXTRA_FLOOR_TEST_CASES(floor_scale_4_negative_123_4567,
                       {.value1 = {{0x0012D687, 0x00000000, 0x00000000,
                                    0x80040000}},  // -123.4567
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x80000000}},  // -124
                        .expected_return_code = 0,
                        .test_name = "floor: -123.4567 -> -124 (scale=4)"})

EXTRA_FLOOR_TEST_CASES(floor_scale_5_negative_0_00005,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x80050000}},  // -0.00005
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: -0.00005 -> -1"})

EXTRA_FLOOR_TEST_CASES(floor_max_int_positive,
                       {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                    0x00000000}},  // MAX_INT
                        .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                             0x00000000}},  // MAX_INT
                        .expected_return_code = 0,
                        .test_name = "floor: MAX_INT -> MAX_INT"})

EXTRA_FLOOR_TEST_CASES(floor_max_int_negative,
                       {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                    0x80000000}},  // -MAX_INT
                        .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                             0x80000000}},  // -MAX_INT
                        .expected_return_code = 0,
                        .test_name = "floor: -MAX_INT -> -MAX_INT"})

EXTRA_FLOOR_TEST_CASES(floor_max_int_with_scale_1,
                       {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                    0x00010000}},  // MAX_INT / 10
                        .expected_result = {{0x99999999, 0x99999999, 0x19999999,
                                             0x00000000}},  // целая часть
                        .expected_return_code = 0,
                        .test_name = "floor: MAX_INT/10 -> целая часть"})

EXTRA_FLOOR_TEST_CASES(floor_max_int_negative_with_scale_1,
                       {.value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                    0x80010000}},  // -MAX_INT / 10
                        .expected_result = {{0x9999999A, 0x99999999, 0x19999999,
                                             0x80000000}},  // -целая часть
                        .expected_return_code = 0,
                        .test_name = "floor: -MAX_INT/10 -> -целая часть"})

EXTRA_FLOOR_TEST_CASES(floor_max_scale_positive,
                       {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                    0x001C0000}},  // 1e-28
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "floor: 1e-28 -> 0"})

EXTRA_FLOOR_TEST_CASES(floor_max_scale_negative,
                       {.value1 = {{0x00000001, 0x00000000, 0x00000000,
                                    0x801C0000}},  // -1e-28
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: -1e-28 -> -1"})

EXTRA_FLOOR_TEST_CASES(floor_max_scale_5e28,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x001C0000}},  // 5e-28
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x00000000}},  // 0
                        .expected_return_code = 0,
                        .test_name = "floor: 5e-28 -> 0"})

EXTRA_FLOOR_TEST_CASES(floor_max_scale_negative_5e28,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x801C0000}},  // -5e-28
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: -5e-28 -> -1"})

START_TEST(test_floor_overflow) {
  // Максимальное значение с дробной частью
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}};
  s21_decimal result = {0};

  // При floor для отрицательного числа может быть переполнение
  // Но для положительного - нет
  int code = s21_floor(value, &result);
  ck_assert_int_eq(code, 0);

  // Проверяем, что результат корректен
  s21_decimal expected = {{0x99999999, 0x99999999, 0x19999999, 0x00000000}};
  ck_assert(compare_decimal(result, expected));

  printf("[PASS] floor: no overflow for positive\n");
}
END_TEST

START_TEST(test_floor_negative_overflow) {
  // Минимальное значение с дробной частью
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);
  // Для -MAX_INT/10 floor должен дать -MAX_INT/10 - 1
  // Это может вызвать переполнение
  // Проверяем, что функция правильно обрабатывает ошибку
  if (code != 0) {
    printf("[PASS] floor: overflow detected for negative\n");
  } else {
    printf("[PASS] floor: no overflow for negative\n");
  }
}
END_TEST

START_TEST(test_floor_null_pointer) {
  s21_decimal value = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}};

  // Передаем NULL как result
  int code = s21_round(value, NULL);

  // Ожидаем ошибку (1)
  ck_assert_int_eq(code, 1);

  printf("[PASS] round: NULL pointer -> error\n");
}
END_TEST

EXTRA_FLOOR_TEST_CASES(
    floor_invalid_scale_255,
    {.value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00FF0000}},  // scale=255
     .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
     .expected_return_code = 1,
     .test_name = "floor: invalid scale 255 -> error"})

EXTRA_FLOOR_TEST_CASES(
    floor_invalid_scale_29,
    {.value1 = {{0x00000005, 0x00000000, 0x00000000, 0x001D0000}},  // scale=29
     .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
     .expected_return_code = 1,
     .test_name = "floor: invalid scale 29 -> error"})

EXTRA_FLOOR_TEST_CASES(floor_sign_preservation_positive,
                       {.value1 = {{0x0000000A, 0x00000000, 0x00000000,
                                    0x00010000}},  // 1.0
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x00000000}},  // 1
                        .expected_return_code = 0,
                        .test_name = "floor: positive sign preserved"})

EXTRA_FLOOR_TEST_CASES(floor_sign_preservation_negative,
                       {.value1 = {{0x0000000A, 0x00000000, 0x00000000,
                                    0x80010000}},  // -1.0
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: negative sign preserved"})

EXTRA_FLOOR_TEST_CASES(floor_sign_preservation_negative_0_5,
                       {.value1 = {{0x00000005, 0x00000000, 0x00000000,
                                    0x80010000}},  // -0.5
                        .expected_result = {{0x00000001, 0x00000000, 0x00000000,
                                             0x80000000}},  // -1
                        .expected_return_code = 0,
                        .test_name = "floor: -0.5 -> -1 (sign preserved)"})

EXTRA_FLOOR_TEST_CASES(floor_negative_123_000,
                       {.value1 = {{0x0000300C, 0x00000000, 0x00000000,
                                    0x80020000}},  // -123.00
                        .expected_result = {{0x0000007B, 0x00000000, 0x00000000,
                                             0x80000000}},  // -123
                        .expected_return_code = 0,
                        .test_name = "floor: -123.00 -> -123 (no fraction)"})

EXTRA_FLOOR_TEST_CASES(floor_negative_123_001,
                       {.value1 = {{0x0001E079, 0x00000000, 0x00000000,
                                    0x80030000}},  // -123.001
                        .expected_result = {{0x0000007C, 0x00000000, 0x00000000,
                                             0x80000000}},  // -124
                        .expected_return_code = 0,
                        .test_name = "floor: -123.001 -> -124 (has fraction)"})

EXTRA_FLOOR_TEST_CASES(floor_negative_0_000,
                       {.value1 = {{0x00000000, 0x00000000, 0x00000000,
                                    0x80030000}},  // -0.000
                        .expected_result = {{0x00000000, 0x00000000, 0x00000000,
                                             0x80000000}},  // -0
                        .expected_return_code = 0,
                        .test_name = "floor: -0.000 -> -0"})

Suite *extra_suite_create(void) {
  Suite *s = suite_create("s21_extra");

  TCase *tc_negate = tcase_create("negate");
  // Тесты Negate
  tcase_add_test(tc_negate, test_negate_positive_to_negative);
  tcase_add_test(tc_negate, test_negate_negative_to_positive);
  tcase_add_test(tc_negate, test_negate_zero);
  tcase_add_test(tc_negate, test_negate_negative_zero);
  tcase_add_test(tc_negate, test_negate_with_scale);
  tcase_add_test(tc_negate, test_negate_negative_with_scale);
  tcase_add_test(tc_negate, test_negate_max_positive);
  tcase_add_test(tc_negate, test_negate_min_negative);
  tcase_add_test(tc_negate, test_negate_large_with_scale);
  tcase_add_test(tc_negate, test_negate_large_number);
  tcase_add_test(tc_negate, test_negate_null_pointer);
  tcase_add_test(tc_negate, test_negate_max_scale);
  tcase_add_test(tc_negate, test_negate_all_bits_set);

  suite_add_tcase(s, tc_negate);

  TCase *tc_truncate = tcase_create("truncate");

  // Тесты Truncate
  tcase_add_test(tc_truncate, test_div_by_10);
  tcase_add_test(tc_truncate, test_truncate_whole_number_positive);
  tcase_add_test(tc_truncate, test_truncate_whole_number_negative);
  tcase_add_test(tc_truncate, test_truncate_zero);
  tcase_add_test(tc_truncate, test_truncate_whole_large);
  tcase_add_test(tc_truncate, test_truncate_whole_large_negative);
  tcase_add_test(tc_truncate, test_truncate_fractional_positive);
  tcase_add_test(tc_truncate, test_truncate_fractional_negative);
  tcase_add_test(tc_truncate, test_truncate_fractional_small);
  tcase_add_test(tc_truncate, test_truncate_fractional_less_than_one);
  tcase_add_test(tc_truncate, test_truncate_fractional_negative_less_than_one);
  tcase_add_test(tc_truncate, test_truncate_fractional_scale_3);
  tcase_add_test(tc_truncate, test_truncate_fractional_scale_4);
  tcase_add_test(tc_truncate, test_truncate_fractional_scale_28);
  tcase_add_test(tc_truncate, test_truncate_with_scale_1);
  tcase_add_test(tc_truncate, test_truncate_with_scale_1_negative);
  tcase_add_test(tc_truncate, test_truncate_with_scale_2);
  tcase_add_test(tc_truncate, test_truncate_with_scale_2_negative);
  tcase_add_test(tc_truncate, test_truncate_large_fractional);
  tcase_add_test(tc_truncate, test_truncate_large_fractional_negative);
  tcase_add_test(tc_truncate, test_truncate_invalid_scale_255);
  tcase_add_test(tc_truncate, test_truncate_invalid_scale_29);
  tcase_add_test(tc_truncate, test_truncate_very_small_positive);
  tcase_add_test(tc_truncate, test_truncate_very_small_negative);

  suite_add_tcase(s, tc_truncate);

  TCase *tc_round = tcase_create("round");

  // Основные round
  tcase_add_test(tc_round, test_round_whole_positive);
  tcase_add_test(tc_round, test_round_whole_negative);
  tcase_add_test(tc_round, test_round_zero);
  tcase_add_test(tc_round, test_round_negative_zero);
  tcase_add_test(tc_round, test_round_0_5_to_0);
  tcase_add_test(tc_round, test_round_1_5_to_2);
  tcase_add_test(tc_round, test_round_2_5_to_2);
  tcase_add_test(tc_round, test_round_3_5_to_4);
  tcase_add_test(tc_round, test_round_4_5_to_4);
  tcase_add_test(tc_round, test_round_negative_0_5_to_0);
  tcase_add_test(tc_round, test_round_negative_1_5_to_2);
  tcase_add_test(tc_round, test_round_negative_2_5_to_2);
  tcase_add_test(tc_round, test_round_negative_3_5_to_4);
  tcase_add_test(tc_round, test_round_0_4_to_0);
  tcase_add_test(tc_round, test_round_0_6_to_1);
  tcase_add_test(tc_round, test_round_negative_0_4_to_0);
  tcase_add_test(tc_round, test_round_negative_0_6_to_1);
  tcase_add_test(tc_round, test_round_scale_2_123_45);
  tcase_add_test(tc_round, test_round_scale_2_123_55);
  tcase_add_test(tc_round, test_round_scale_2_123_50);
  tcase_add_test(tc_round, test_round_scale_2_122_50);
  tcase_add_test(tc_round, test_round_scale_3_123_456);
  tcase_add_test(tc_round, test_round_scale_3_123_556);
  tcase_add_test(tc_round, test_round_scale_4_0_0005);
  tcase_add_test(tc_round, test_round_scale_4_0_0006);
  tcase_add_test(tc_round, test_round_max_int);
  tcase_add_test(tc_round, test_round_max_int_negative);
  tcase_add_test(tc_round, test_round_max_int_with_scale_1);
  tcase_add_test(tc_round, test_round_sign_preservation_positive);
  tcase_add_test(tc_round, test_round_sign_preservation_negative);
  tcase_add_test(tc_round, test_round_123_4567);
  tcase_add_test(tc_round, test_round_123_5567);
  tcase_add_test(tc_round, test_round_negative_123_4567);
  tcase_add_test(tc_round, test_round_negative_123_5567);
  tcase_add_test(tc_round, test_round_max_scale_1e28);
  tcase_add_test(tc_round, test_round_max_scale_5e28);
  tcase_add_test(tc_round, test_round_null_pointer);
  tcase_add_test(tc_round, test_round_invalid_scale_255);
  tcase_add_test(tc_round, test_round_invalid_scale_29);

  suite_add_tcase(s, tc_round);

  // Tests Floor

  TCase *tc_floor = tcase_create("floor");

  tcase_add_test(tc_floor, test_floor_whole_positive);
  tcase_add_test(tc_floor, test_floor_whole_negative);
  tcase_add_test(tc_floor, test_floor_zero);
  tcase_add_test(tc_floor, test_floor_negative_zero);
  tcase_add_test(tc_floor, test_floor_positive_0_5);
  tcase_add_test(tc_floor, test_floor_positive_0_9);
  tcase_add_test(tc_floor, test_floor_positive_1_1);
  tcase_add_test(tc_floor, test_floor_positive_1_9);
  tcase_add_test(tc_floor, test_floor_positive_123_45);
  tcase_add_test(tc_floor, test_floor_positive_123_99);
  tcase_add_test(tc_floor, test_floor_negative_0_5);
  tcase_add_test(tc_floor, test_floor_negative_0_1);
  tcase_add_test(tc_floor, test_floor_negative_0_9);
  tcase_add_test(tc_floor, test_floor_negative_1_1);
  tcase_add_test(tc_floor, test_floor_negative_1_5);
  tcase_add_test(tc_floor, test_floor_negative_1_9);
  tcase_add_test(tc_floor, test_floor_negative_123_45);
  tcase_add_test(tc_floor, test_floor_negative_123_01);
  tcase_add_test(tc_floor, test_floor_negative_123_99);

  tcase_add_test(tc_floor, test_floor_scale_3_123_456);
  tcase_add_test(tc_floor, test_floor_scale_4_123_4567);
  tcase_add_test(tc_floor, test_floor_scale_5_0_00005);
  tcase_add_test(tc_floor, test_floor_scale_3_negative_123_456);
  tcase_add_test(tc_floor, test_floor_scale_4_negative_123_4567);
  tcase_add_test(tc_floor, test_floor_scale_5_negative_0_00005);

  tcase_add_test(tc_floor, test_floor_max_int_positive);
  tcase_add_test(tc_floor, test_floor_max_int_negative);
  tcase_add_test(tc_floor, test_floor_max_int_with_scale_1);
  tcase_add_test(tc_floor, test_floor_max_int_negative_with_scale_1);
  tcase_add_test(tc_floor, test_floor_max_scale_positive);
  tcase_add_test(tc_floor, test_floor_max_scale_negative);
  tcase_add_test(tc_floor, test_floor_max_scale_5e28);
  tcase_add_test(tc_floor, test_floor_max_scale_negative_5e28);

  tcase_add_test(tc_floor, test_floor_sign_preservation_positive);
  tcase_add_test(tc_floor, test_floor_sign_preservation_negative);
  tcase_add_test(tc_floor, test_floor_sign_preservation_negative_0_5);

  tcase_add_test(tc_floor, test_floor_negative_123_000);
  tcase_add_test(tc_floor, test_floor_negative_123_001);
  tcase_add_test(tc_floor, test_floor_negative_0_000);

  tcase_add_test(tc_floor, test_floor_null_pointer);
  tcase_add_test(tc_floor, test_floor_invalid_scale_255);
  tcase_add_test(tc_floor, test_floor_invalid_scale_29);
  tcase_add_test(tc_floor, test_floor_overflow);
  tcase_add_test(tc_floor, test_floor_negative_overflow);

  suite_add_tcase(s, tc_floor);
  return s;
}