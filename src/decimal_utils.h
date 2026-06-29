#ifndef DECIMAL_UTILS_H
#define DECIMAL_UTILS_H

#include "s21_decimal.h"
#define LAST_BIT 31
#define SINGLE_BIT 1

void align_scales(s21_decimal * value_1, s21_decimal * value_2);
int get_sign(int service_bits);
int get_scale(int service_bits);
int compare_mantissas(s21_decimal value_1, s21_decimal value_2);
int substract_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void set_sign(s21_decimal *value, int sign);
void set_scale(s21_decimal *value, int scale);
int s21_add_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal * result);
int shift_left(s21_decimal * value, int bits_count);
int shift_left_1_bit(s21_decimal * value);
int mul_by_10(s21_decimal * value);
int decimal_is_zero(s21_decimal value);

#endif
