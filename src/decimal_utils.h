#include "s21_decimal.h"
<<<<<<< HEAD
=======
#define LAST_BIT 31
#define SINGLE_BIT 1
>>>>>>> 8df418083bdde2e4cef1d91efe8a4a3dd977a8b8

void align_scales(s21_decimal * value_1,s21_decimal * value_2);
int get_sign(int service_bits);
int get_scale(int service_bits);
int compare_mantissas(s21_decimal value_1, s21_decimal value_2);
<<<<<<< HEAD
int will_addition_overflow(s21_decimal value_1, s21_decimal value_2);
int substract_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void set_sign(s21_decimal *value, int sign);
void set_scale(s21_decimal *value, int scale);
=======
int substract_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void set_sign(s21_decimal *value, int sign);
void set_scale(s21_decimal *value, int scale);
int s21_add_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal * result);
void shift_left(s21_decimal * value, int bits_count);
int shift_left_1_bit(s21_decimal * value);
int mul_by_10(s21_decimal * value);
void align_scales(s21_decimal * value_1, s21_decimal * value_2);
>>>>>>> 8df418083bdde2e4cef1d91efe8a4a3dd977a8b8
