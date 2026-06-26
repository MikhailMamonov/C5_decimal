#include "../s21_decimal.h"

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

typedef struct {
  const s21_decimal value1;
  const s21_decimal value2;
  const s21_decimal expected_result;
  int expected_return_code; 
  const char *test_name;
} addParams;

typedef struct {
  const s21_decimal value1;
  const s21_decimal value2;
  const s21_decimal expected_result;
  int expected_return_code; 
  const char *test_name;
} subParams;

typedef struct {
  const char *src;
  const char *trim_chars;
  const char *expected;
  const char *test_name;
} mulParams;

typedef struct {
  const char *str;
  const char *expected;
  const char *test_name;
} divParams;

#define TEST_CASES(name, param_type, run_func, ...)                      \
  static param_type name[] = {__VA_ARGS__};                              \
  START_TEST(test_##name) {                                              \
    for (size_t i = 0; i < sizeof(name) / sizeof(param_type); i++) {     \
      run_func(&name[i]);                                                \
    }                                                                    \
  }                                                                      \
  END_TEST

#define ADD_TEST_CASES(name, ...) \
  TEST_CASES(name, addParams, run_add_test, __VA_ARGS__)

#define SUB_TEST_CASES(name, ...) \
  TEST_CASES(name, subParams, run_sub_test, __VA_ARGS__)

#define MUL_TEST_CASES(name, ...) \
  TEST_CASES(name, mulParams, run_mul_test)

#define DIV_TEST_CASES(name, ...) \
  TEST_CASES(name, divParams, run_div_test)

#endif
