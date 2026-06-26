/*#include <check.h>
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

static void run_div_test(addParams *params) {
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

// Основные случаи: разный регистр, цифры, символы
DIV_TEST_CASES(sub_positive, {
  .value1 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}}, // 1
  .value2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}}, // 2
  .expected_result = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}}, // 3
  .expected_return_code = 0,
  .test_name = "1 + 2 = 3"
})

DIV_TEST_CASES(sub_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .value2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}}, // -3
  .expected_result = {{0x00000008, 0x00000000, 0x00000000, 0x80000000}}, // -8
  .expected_return_code = 0,
  .test_name = "-5 + (-3) = -8"
})

DIV_TEST_CASES(sub_mixed_positive, {
  .value1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}}, // 10
  .value2 = {{0x00000004, 0x00000000, 0x00000000, 0x80000000}}, // -4
  .expected_result = {{0x00000006, 0x00000000, 0x00000000, 0x00000000}}, // 6
  .expected_return_code = 0,
  .test_name = "10 + (-4) = 6"
})


DIV_TEST_CASES(sub_mixed_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .value2 = {{0x0000000C, 0x00000000, 0x00000000, 0x80000000}}, // -12
  .expected_result = {{0x00000007, 0x00000000, 0x00000000, 0x80000000}}, // -7
  .expected_return_code = 0,
  .test_name = "5 + (-12) = -7"
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
}*/
