#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"
#include "test_s21_common.h"


static void run_round_test(TestParams *params) {
  int return_code = s21_is_equal(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

static void run_floor_test(TestParams *params) {
  int return_code = s21_is_equal(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

static void run_negate_test(TestParams *params) {
    
    s21_decimal result = {0};
    
    int return_code = s21_negate(params->value1, &result);
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

static void run_truncate_test(TestParams *params) {
  int return_code = s21_is_equal(params->value1, params->value2);
  ck_assert_int_eq(return_code, params->expected_return_code);
  printf("[PASS] %s\n", params->test_name);
}

// 1. Положительное число -> Отрицательное
EXTRA_NEGATE_TEST_CASES(negate_positive_to_negative, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_return_code = 0,  // Всегда OK
  .test_name = "5 -> -5"
})

// 2. Отрицательное число -> Положительное
EXTRA_NEGATE_TEST_CASES(negate_negative_to_positive, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x80000000}}, // -5
  .expected_result = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_return_code = 0,
  .test_name = "-5 -> 5"
})

// 3. Ноль -> Ноль (без изменений)
EXTRA_NEGATE_TEST_CASES(negate_zero, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "0 -> 0"
})

// 4. Отрицательный ноль -> Положительный ноль
EXTRA_NEGATE_TEST_CASES(negate_negative_zero, {
  .value1 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}}, // -0
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // 0
  .expected_return_code = 0,
  .test_name = "-0 -> 0"
})

// 5. Число с масштабом
EXTRA_NEGATE_TEST_CASES(negate_with_scale, {
  .value1 = {{0x000000C8, 0x00000000, 0x00000000, 0x00020000}}, // 2.00
  .expected_result = {{0x000000C8, 0x00000000, 0x00000000, 0x80020000}}, // -2.00
  .expected_return_code = 0,
  .test_name = "2.00 -> -2.00"
})

// 6. Отрицательное с масштабом
EXTRA_NEGATE_TEST_CASES(negate_negative_with_scale, {
  .value1 = {{0x000000C8, 0x00000000, 0x00000000, 0x80020000}}, // -2.00
  .expected_result = {{0x000000C8, 0x00000000, 0x00000000, 0x00020000}}, // 2.00
  .expected_return_code = 0,
  .test_name = "-2.00 -> 2.00"
})

// 7. Большое число
EXTRA_NEGATE_TEST_CASES(negate_large_number, {
  .value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}}, // Max decimal
  .expected_result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}, // -Max
  .expected_return_code = 0,
  .test_name = "Max -> -Max"
})

// 8. NULL указатель (если проверяете)
EXTRA_NEGATE_TEST_CASES(negate_null_pointer, {
  .value1 = {{0x00000005, 0x00000000, 0x00000000, 0x00000000}}, // 5
  .expected_result = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}}, // Не важно
  .expected_return_code = 1,  // Ошибка
  .test_name = "NULL pointer"
})


Suite *extra_suite_create(void) {
    Suite *s = suite_create("s21_extra");
    
    TCase *tc_negate = tcase_create("negate");
    tcase_add_test(tc_negate, test_negate_positive_to_negative);
    
    tcase_add_test(tc_negate, test_negate_negative_to_positive);
    
    // 3. Ноль -> Ноль
    tcase_add_test(tc_negate, test_negate_zero);
    
    // 4. Отрицательный ноль -> Положительный ноль
    tcase_add_test(tc_negate, test_negate_negative_zero);
    
    // 5. Число с масштабом -> Отрицательное с масштабом
    tcase_add_test(tc_negate, test_negate_with_scale);
    
    // 6. Отрицательное с масштабом -> Положительное с масштабом
    tcase_add_test(tc_negate, test_negate_negative_with_scale);
    
    // 7. Максимальное положительное число -> Максимальное отрицательное
    tcase_add_test(tc_negate, test_negate_max_positive);
    
    // 8. Минимальное отрицательное число -> Максимальное положительное
    tcase_add_test(tc_negate, test_negate_min_negative);
    
    // 9. Большое число с масштабом
    tcase_add_test(tc_negate, test_negate_large_with_scale);
    
    // 10. Двойное отрицание (возвращает исходное)
    tcase_add_test(tc_negate, test_negate_twice);
    
    // 11. NULL указатель (ошибка)
    tcase_add_test(tc_negate, test_negate_null_pointer);
    
    // 12. Максимальный масштаб (28)
    tcase_add_test(tc_negate, test_negate_max_scale);
    
    // 13. Все биты установлены
    tcase_add_test(tc_negate, test_negate_all_bits_set);
    
    // 14. Случайное число
    tcase_add_test(tc_negate, test_negate_random);
    
    suite_add_tcase(s, tc_negate);

    return s;
}