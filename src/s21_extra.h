#ifndef S21_EXTRA_H
#define S21_EXTRA_H

// Другие функции (Other functions)
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif // S21_EXTRA_H