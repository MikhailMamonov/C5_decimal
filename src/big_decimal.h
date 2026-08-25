#ifndef BIG_DECIMAL_H
#define BIG_DECIMAL_H

#include "s21_decimal.h"
#define BIG_SIGNIFICANT_BYTES 6
#define BIG_ELDER_BIT 191

typedef struct {
  int bits[7];
} big_decimal;

big_decimal assign(s21_decimal);
s21_decimal assign_back(big_decimal value);
int big_get_bit_val(big_decimal value, int idx);
int big_add_mantissas(big_decimal value_1, big_decimal value_2,
                      big_decimal *result);
int big_shift_left_1_bit(big_decimal *value);
int fits_in_decimal(big_decimal value);
int big_div_by_10(big_decimal *num);
int big_add_one(big_decimal *value);
int equals_zero(big_decimal value);
int big_mul_by_10(big_decimal *value);
int big_is_greater_or_equal(big_decimal value_1, big_decimal value_2);
int check_result(big_decimal *local_result, int *res_scale, int fail);
int big_substract_mantissas(big_decimal value_1, big_decimal value_2,
                            big_decimal *result);

#endif