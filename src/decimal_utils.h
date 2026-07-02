#ifndef DECIMAL_UTILS_H
#define DECIMAL_UTILS_H

#include "s21_decimal.h"

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
int div_by_10(s21_decimal *num);
int add_one(s21_decimal *num);
int get_bit_val(s21_decimal value, int idx);
int decimal_is_zero(s21_decimal value);
int set_byte(int num, int byte_index, unsigned char new_byte);

#endif
