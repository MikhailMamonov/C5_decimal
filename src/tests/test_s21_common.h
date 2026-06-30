#include "../s21_decimal.h"

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

typedef struct {
  const s21_decimal value1;
  const s21_decimal value2;
  const s21_decimal expected_result;
  int expected_return_code; 
  const char *test_name;
} TestParams;

#define TEST_CASES(name, param_type, run_func, ...)                      \
  static param_type name[] = {__VA_ARGS__};                              \
  START_TEST(test_##name) {                                              \
    for (size_t i = 0; i < sizeof(name) / sizeof(param_type); i++) {     \
      run_func(&name[i]);                                                \
    }                                                                    \
  }                                                                      \
  END_TEST

#define ADD_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_add_test, __VA_ARGS__)

#define SUB_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_sub_test, __VA_ARGS__)

#define MUL_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_mul_test, __VA_ARGS__)

#define DIV_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_div_test, __VA_ARGS__)

void decimal_to_string(const s21_decimal a, char * buffer);
int compare_decimal(const s21_decimal a, const s21_decimal b);

#endif
