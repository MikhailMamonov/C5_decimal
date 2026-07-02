#ifndef CONVERTORS_H
#define CONVERTORS_H

#include "s21_decimal.h"
#define ERROR 1
#define MAX_FLOAT_PRECISION 7
#define MIN_INT -2147483648
#define MAX_INT 0x80000000U
#define POWER_32 4294967296.0L          // Это 2^32 в формате long double
#define POWER_64 18446744073709551616.0L // Это 2^64 в формате long double

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

#endif