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

static void run_div_test(TestParams *params) {
  s21_decimal result = {{0}};

  int return_code = s21_add(params->value1, params->value2, &result);

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

DIV_TEST_CASES(div_basic, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2
  .expected_result = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_return_code = 0,
  .test_name = "10 / 2 = 5"
})

DIV_TEST_CASES(div_different_signs, {
  .value1 = {{0x00000014, 0x00000000, 0x00000000, 0x00000000}}, // 20
  .value2 = {{0x00000004, 0x00000000, 0x00000000, 0x80000000}}, // -4
  .expected_result = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_return_code = 0,
  .test_name = "20 / (-4) = -5"
})

DIV_TEST_CASES(div_two_negatives, {
  .value1 = {{0x00000032, 0x00000000, 0x00000000, 0x80000000}}, // -50
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .expected_return_code = 0,
  .test_name = "(-50) / (-5) = 10"
})

DIV_TEST_CASES(div_by_one_preserves_scale, {
  .value1 = {{0x0000007B, 0x00000000, 0x00000000, 0x00020000}}, // 1.23 (scale 2)
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1 (scale 0)
  .expected_result = {{0x0000007B, 0x00000000, 0x00000000, 0x00020000}}, // 1.23 (scale 2 - 0 = 2)
  .expected_return_code = 0,
  .test_name = "1.23 / 1 = 1.23"
})

DIV_TEST_CASES(div_zero_by_number, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .value2 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0 / 5 = 0"
})

DIV_TEST_CASES(div_by_zero_error, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 3, // FAIL_DIVIDE_ZERO
  .test_name = "5 / 0 = FAIL_DIVIDE_ZERO"
})

DIV_TEST_CASES(div_fractional_loop, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  // 1/3 = 0.3333333333333333333333333333 (28 троек, масштаб 28)
  // В шестнадцатеричной системе мантисса 28 троек равна 0x1A14E3BC 0x2AFA0E5B 0x3D75A035
  .expected_result = {{0x2AFA0E5B, 0x3D75A035, 0x1A14E3BC, 0x001C0000}}, // 28 троек, scale 28
  .expected_return_code = 0,
  .test_name = "1 / 3 = 0.333... (max scale generation)"
})

DIV_TEST_CASES(div_bankers_round_up, {
  // 3e-28 / 2 = 1.5e-28 -> в алгоритме превратится в 15e-29.
  // check_result урежет масштаб до 28, получит мантиссу 1 и остаток 5.
  // Нечётная 1 округлится по банковскому правилу вверх до ближайшего чётного — до 2!
  .value1 = {{0x00000003, 0x00000000, 0x00000000, 0x001C0000}}, // 3e-28 (scale 28)
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2 (scale 0)
  .expected_result = {{0x00000002, 0x00000000, 0x00000000, 0x001C0000}}, // 2e-28 (scale 28)
  .expected_return_code = 0,
  .test_name = "3e-28 / 2 = 2e-28 (bankers round up)"
})

DIV_TEST_CASES(div_bankers_round_hold, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x001C0000}}, // 5e-28
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2
  // 5e-28 / 2 = 2.5e-28 -> в цикле do-while это 25e-29.
  // check_result делит 25 на 10 -> получаем 2, остаток 5.
  // Двойка уже четная -> оставляем как есть!
  .expected_result = {{0x00000002, 0x00000000, 0x00000000, 0x001C0000}}, // 2e-28 (scale 28)
  .expected_return_code = 0,
  .test_name = "5e-28 / 2 = 2e-28 (bankers round hold)"
})

DIV_TEST_CASES(div_overflow_to_infinity, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // MAX_DECIMAL
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1 (scale 1, мантисса 1)
  // MAX_DECIMAL / 0.1 = MAX_DECIMAL * 10 -> гарантированный взрыв в плюс бесконечность
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 1, // FAIL_TOO_LARGE
  .test_name = "MAX_DECIMAL / 0.1 = Overflow (+inf)"
})

DIV_TEST_CASES(div_overflow_to_negative_infinity, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}, // -MAX_DECIMAL
  .value2 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}}, // 0.1 (scale 1, мантисса 1)
  // -MAX_DECIMAL / 0.1 = -MAX_DECIMAL * 10 -> гарантированный взрыв в минус бесконечность
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},
  .expected_return_code = 2, // FAIL_TOO_SMALL
  .test_name = "(-MAX_DECIMAL) / 0.1 = Underflow (-inf)"
})



Suite *div_suite_create(void) {
  Suite *s = suite_create("Division");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_div_basic);
  tcase_add_test(tc, test_div_different_signs);
  tcase_add_test(tc, test_div_two_negatives);
  tcase_add_test(tc, test_div_by_one_preserves_scale);
  tcase_add_test(tc, test_div_zero_by_number);
  tcase_add_test(tc, test_div_by_zero_error);
  tcase_add_test(tc, test_div_fractional_loop);
  tcase_add_test(tc, test_div_bankers_round_up);
  tcase_add_test(tc, test_div_bankers_round_hold);
  tcase_add_test(tc, test_div_overflow_to_infinity);
  tcase_add_test(tc, test_div_overflow_to_negative_infinity);
  suite_add_tcase(s, tc);
  return s;
}
