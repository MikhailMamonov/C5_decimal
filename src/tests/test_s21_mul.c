#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"

static int compare_decimal(const s21_decimal *a, const s21_decimal *b){
    for (int i=0;i<COUNT_OF_BITS;i++){
        if(a->bits[i]!=b->bits[i]){
            return 0;
        }
    }

    return 1;
}

static void run_mul_test(TestParams *params) {
  s21_decimal result = {{0}};

  int return_code = s21_mul(params->value1, params->value2, &result);

  ck_assert_int_eq(return_code, params->expected_return_code);

  if(return_code==SUCCESS){
    ck_assert_msg(compare_decimal(&result, &params->expected_result),
        "test '%s' failed: result not match with expected.",
        params->test_name);
    ck_assert_ptr_ne((void *)&result, (void *)&params->value1);
    ck_assert_ptr_ne((void *)&result, (void *)&params->value2);
  }
  printf("[PASS] %s\n", params->test_name);
}

MUL_TEST_CASES(mul_basic_positives, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000006, 0x00000000, 0x00000000, 0x00000000}}, // 6
  .expected_result = {{0x0000001E, 0x00000000, 0x00000000, 0x00000000}}, // 30
  .expected_return_code = 0,
  .test_name = "5 * 6 = 30"
})

MUL_TEST_CASES(mul_positive_and_negative, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .expected_result = {{0x0000001E, 0x00000000, 0x00000000, 0x80000000}}, // -30
  .expected_return_code = 0,
  .test_name = "10 * (-3) = -30"
})

MUL_TEST_CASES(mul_two_negatives, {
  .value1 = {{0x00000004, 0x00000000, 0x00000000, 0x80000000}}, // -4
  .value2 = {{0x00000004, 0x00000000, 0x00000000, 0x80000000}}, // -4
  .expected_result = {{0x00000010, 0x00000000, 0x00000000, 0x00000000}}, // 16
  .expected_return_code = 0,
  .test_name = "(-4) * (-4) = 16"
})

MUL_TEST_CASES(mul_by_zero, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX_DECIMAL
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "MAX_DECIMAL * 0 = 0"
})

MUL_TEST_CASES(mul_scales_addition, {
  .value1 = {{0x0000000C, 0x00000000, 0x00000000, 0x00010000}}, // 1.2 (scale 1, мантисса 12)
  .value2 = {{0x00000019, 0x00000000, 0x00000000, 0x00020000}}, // 0.25 (scale 2, мантисса 25)
  // 12 * 25 = 300, scale 1 + 2 = 3. Итого: 300 / 10^3 = 0.300
  .expected_result = {{0x0000012C, 0x00000000, 0x00000000, 0x00030000}}, // 0.300 (scale 3, мантисса 300)
  .expected_return_code = 0,
  .test_name = "1.2 * 0.25 = 0.300 (scales added)"
})

MUL_TEST_CASES(mul_large_bits_overflow_to_int_blocks, {
  // Проверяет правильность переноса битов («домино») между bits[0], bits[1] и bits[2]
  .value1 = {{0x00000000, 0x00000001, 0x00000000, 0x00000000}}, // 2^32 (единица в нулевом бите bits[1])
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2
  .expected_result = {{0x00000000, 0x00000002, 0x00000000, 0x00000000}}, // 2^33
  .expected_return_code = 0,
  .test_name = "2^32 * 2 = 2^33 (multi-block transfer)"
})

MUL_TEST_CASES(mul_overflow_to_infinity, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX_DECIMAL
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1, // FAIL_TOO_LARGE
  .test_name = "MAX_DECIMAL * 2 = Overflow (+inf)"
})

MUL_TEST_CASES(mul_overflow_to_negative_infinity, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}, // -MAX_DECIMAL
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 2, // FAIL_TOO_SMALL (динамический статус fail, который мы настроили)
  .test_name = "(-MAX_DECIMAL) * 2 = Underflow (-inf)"
})

MUL_TEST_CASES(mul_overflow_scale_bankers_rounding, {
  .value1 = {{0x00000019, 0x00000000, 0x00000000, 0x001C0000}}, // 25e-28 (scale 28)
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00010000}}, // 0.2 (scale 1, мантисса 2)
  // 25 * 2 = 50, scale 28 + 1 = 29. 
  // 29 > 28 -> делим 50 на 10 -> получаем 5 с остатком 0. scale падает до 28.
  .expected_result = {{0x00000005, 0x00000000, 0x00000000, 0x001C0000}}, // 5e-28 (scale 28)
  .expected_return_code = 0,
  .test_name = "25e-28 * 0.2 = 5e-28 (scale compressed from 29 to 28)"
})

MUL_TEST_CASES(mul_bankers_round_up_to_even, {
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x001C0000}}, // 3e-28 (scale 28)
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00010000}}, // 0.5 (scale 1, мантисса 5)
  // 3 * 5 = 15, scale 28 + 1 = 29.
  // 29 > 28 -> делим 15 на 10 -> получаем 1, остаток 5.
  // 1 нечётное -> округляем вверх до 2. scale падает до 28.
  .expected_result = {{0x00000002, 0x00000000, 0x00000000, 0x001C0000}}, // 2e-28 (scale 28)
  .expected_return_code = 0,
  .test_name = "3e-28 * 0.5 = 2e-28 (round up to even)"
})

MUL_TEST_CASES(mul_bankers_round_hold_even, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x001C0000}}, // 5e-28 (scale 28)
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00010000}}, // 0.5 (scale 1, мантисса 5)
  // 5 * 5 = 25, scale 28 + 1 = 29.
  // 29 > 28 -> делим 25 на 10 -> получаем 2, остаток 5.
  // 2 уже чётное -> оставляем без изменений. scale падает до 28.
  .expected_result = {{0x00000002, 0x00000000, 0x00000000, 0x001C0000}}, // 2e-28 (scale 28)
  .expected_return_code = 0,
  .test_name = "5e-28 * 0.5 = 2e-28 (hold because already even)"
})

MUL_TEST_CASES(mul_by_minus_zero, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00020000}}, // 0.05 (scale 2)
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x80010000}}, // -0.0 (scale 1)
  // 5 * 0 = 0. scale 2 + 1 = 3. Знак плюс на минус дает минус.
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x80030000}}, // -0.000 (scale 3)
  .expected_return_code = 0,
  .test_name = "0.05 * (-0.0) = -0.000"
})

MUL_TEST_CASES(mul_intermediate_overflow_with_compression, {
  // Два огромных числа, которые не перемножить без big_decimal, но у них есть масштаб
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000A0000}}, // MAX_DECIMAL со scale 10
  .value2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}}, // 1.0 (мантисса 10, scale 1)
  // Мантисса увеличится в 10 раз и вылетит за 96 бит. Но scale станет 11.
  // Цикл урезания масштаба сожмет число обратно до MAX_DECIMAL со scale 10.
  .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000A0000}}, 
  .expected_return_code = 0,
  .test_name = "Large numbers intermediate overflow with successful compression"
})

MUL_TEST_CASES(mul_accumulated_trailing_tail, {
  // Тест на число ...251 со scale 30. Проверяет накопление флага exist_not_zero.
  // Подбираем мантиссы так, чтобы результат умножения был равен ровно 251
  .value1 = {{0x000000FB, 0x00000000, 0x00000000, 0x001C0000}}, // 251e-28 (scale 28)
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00020000}}, // 0.01 (scale 2, мантисса 1)
  // 251 * 1 = 251, scale 28 + 2 = 30.
  // Шаг 1: 251 / 10 = 25, остаток 1 (флаг взведен). scale = 29.
  // Шаг 2: 25 / 10 = 2, остаток 5. Из-за флага чётная двойка округляется вверх до 3!
  .expected_result = {{0x00000003, 0x00000000, 0x00000000, 0x001C0000}}, // 3e-28 (scale 28)
  .expected_return_code = 0,
  .test_name = "251e-28 * 0.01 = 3e-28 (accumulated trailing tail rounds up even digit)"
})

MUL_TEST_CASES(mul_absolute_overflow_with_scale, {
  // value1 = MAX_DECIMAL со scale 1
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, 
  // value2 = MAX_DECIMAL со scale 1
  .value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}}, 
  // Результат гарантированно переполнится даже после сжатия масштаба до 0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1, // FAIL_TOO_LARGE
  .test_name = "Absolute overflow even after scale compression to zero"
})

MUL_TEST_CASES(mul_underflow_to_pure_zero, {
  // Перемножаем экстремально маленькие числа. Мантисса сожмется в ноль.
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x001C0000}}, // 1e-28
  // 1 * 1 = 1, scale = 56. Цикл сделает 28 делений. Единица превратится в 0.
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x001C0000}}, // 0e-28
  .expected_return_code = 0,
  .test_name = "Extreme small numbers multiplication underflows to pure zero"
})



Suite *mul_suite_create(void) {
  Suite *s = suite_create("Multiplication");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_mul_basic_positives);
  tcase_add_test(tc, test_mul_positive_and_negative);
  tcase_add_test(tc, test_mul_two_negatives);
  tcase_add_test(tc, test_mul_by_zero);
  tcase_add_test(tc, test_mul_scales_addition);
  tcase_add_test(tc, test_mul_large_bits_overflow_to_int_blocks);
  tcase_add_test(tc, test_mul_overflow_to_infinity);
  tcase_add_test(tc, test_mul_overflow_to_negative_infinity);
  tcase_add_test(tc, test_mul_bankers_round_up_to_even);
  tcase_add_test(tc, test_mul_bankers_round_up_to_even);
  tcase_add_test(tc, test_mul_bankers_round_hold_even);
  tcase_add_test(tc, test_mul_overflow_scale_bankers_rounding);
  tcase_add_test(tc, test_mul_by_minus_zero);
  tcase_add_test(tc, test_mul_intermediate_overflow_with_compression);
  tcase_add_test(tc, test_mul_accumulated_trailing_tail);
  tcase_add_test(tc, test_mul_absolute_overflow_with_scale);
  tcase_add_test(tc, test_mul_underflow_to_pure_zero);
  suite_add_tcase(s, tc);
  return s;
}

