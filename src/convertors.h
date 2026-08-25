#ifndef CONVERTORS_H
#define CONVERTORS_H

#include "s21_decimal.h"
#define ERROR 1
#define MAX_FLOAT_PRECISION 7
#define MIN_INT -2147483648
#define MAX_INT 0x80000000U
#define MAX_DECIMAL 79228162514264337593543950335.0f

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

#endif