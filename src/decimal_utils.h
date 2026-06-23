#include "s21_decimal.h"

void align_scales(s21_decimal * value_1,s21_decimal * value_2);
int get_sign(int service_bits);
int get_scale(int service_bits);
int compare_mantissas(s21_decimal value_1, s21_decimal value_2);