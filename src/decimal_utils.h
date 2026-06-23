#include "s21_decimal.h"

void align_scales(s21_decimal * value_1,s21_decimal * value_2);
int get_sign(int service_bits);
int get_scale(int service_bits);
int compare_mantissas(s21_decimal value_1, s21_decimal value_2);
int will_addition_overflow(s21_decimal value_1, s21_decimal value_2);
int substract_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void set_sign(s21_decimal *value, int sign);
void set_scale(s21_decimal *value, int scale);