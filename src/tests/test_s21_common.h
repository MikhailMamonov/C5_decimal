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

typedef enum {
    COMPARE_EQUAL,
    COMPARE_NOT_EQUAL,
    COMPARE_LESS,
    COMPARE_LESS_OR_EQUAL,
    COMPARE_GREATER,
    COMPARE_GREATER_OR_EQUAL
} CompareType;

void decimal_to_string(s21_decimal dec, char *str);
int compare_decimal(s21_decimal a, s21_decimal b);

typedef struct {
  const int src;
  const s21_decimal expected_result;
  int expected_return_code; 
  const char *test_name;
} fromIntParams;

typedef struct {
  const float src;
  const s21_decimal expected_result;
  int expected_return_code; 
  const char *test_name;
} fromFloatParams;

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

// Отдельные макросы для каждой функции сравнения
#define COMPARE_EQUAL_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_compare_equal_test, __VA_ARGS__)

#define COMPARE_NOT_EQUAL_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_compare_not_equal_test, __VA_ARGS__)

#define COMPARE_LESS_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_compare_less_test, __VA_ARGS__)

#define COMPARE_LESS_OR_EQUAL_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_compare_less_or_equal_test, __VA_ARGS__)

#define COMPARE_GREATER_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_compare_greater_test, __VA_ARGS__)

#define COMPARE_GREATER_OR_EQUAL_TEST_CASES(name, ...) \
  TEST_CASES(name, TestParams, run_compare_greater_or_equal_test, __VA_ARGS__)
#define FROM_INT_TEST_CASES(name, ...) \
  TEST_CASES(name, fromIntParams, run_from_int_test, __VA_ARGS__)

#define FROM_FLOAT_TEST_CASES(name, ...) \
  TEST_CASES(name, fromFloatParams, run_from_float_test, __VA_ARGS__)

#endif
