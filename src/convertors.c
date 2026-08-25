#include "convertors.h"

#include "ctype.h"
#include "decimal_utils.h"
#include "math.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst == NULL) {
    return ERROR;
  }
  for (int i = 0; i <= SIGNIFICANT_BYTES; i++) {
    dst->bits[i] = 0;
  }
  unsigned int true_src = (unsigned int)src;
  if (src < 0) {
    set_sign(dst, SIGN_NEGATIVE);
    true_src = -(unsigned int)(src);
  }
  dst->bits[0] = true_src;

  return SUCCESS;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (dst == NULL || fabs(src) > MAX_DECIMAL) {
    return ERROR;
  }
  if (fabs(src) > 0 && fabs(src) < 1e-28) {
    src = 0;
    return ERROR;
  }
  for (int i = 0; i <= SIGNIFICANT_BYTES; i++) {
    dst->bits[i] = 0;
  }
  if (src < 0) {
    set_sign(dst, SIGN_NEGATIVE);
  }
  int exp_presicion = MAX_FLOAT_PRECISION - 1;
  char *str = malloc(sizeof(char) * (MAX_SCALE));
  if (str == NULL) {
    return ERROR;
  }
  char *start = str;
  sprintf(str, "%.6e", src);
  int exponent = 0, is_exponent = 0, exp_sign = 1;
  while (*str != '\0') {
    s21_decimal digit = {0};
    if (isdigit(*str)) {
      if (is_exponent) {
        exponent *= 10;
        exponent += (*str - '0');
      } else {
        s21_from_int_to_decimal(*str - '0', &digit);
        mul_by_10(dst);
        s21_add_mantissas(*dst, digit, dst);
      }
    }
    if (is_exponent && *str == '-') {
      exp_sign = -1;
    }
    if (*str == 'e') {
      is_exponent = 1;
    }
    str++;
  }
  exponent *= exp_sign;
  int res_scale = exp_presicion - exponent;
  while (res_scale < 0) {
    mul_by_10(dst);
    res_scale++;
  }
  set_scale(dst, res_scale);

  free(start);
  return SUCCESS;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (dst == NULL) {
    return ERROR;
  }
  int scale = get_scale(src.bits[SIGN_BYTE_IDX]);
  while (scale > 0) {
    div_by_10(&src);
    scale--;
  }
  int return_code = SUCCESS;
  for (int i = 1; i < SIGNIFICANT_BYTES; i++) {
    if (src.bits[i] != 0) {
      return_code = ERROR;
      break;
    }
  }
  if (get_bit_val(src, SIGN_BIT) != 0 &&
      get_sign(src.bits[SIGN_BYTE_IDX]) == SIGN_POSITIVE) {
    return_code = ERROR;
  }
  unsigned int temp = (unsigned int)src.bits[0];
  if (temp > MAX_INT) {
    return_code = ERROR;
  }
  if (!return_code) {
    *dst = (int)temp;
  }
  if (*dst < 0 && get_sign(src.bits[SIGN_BYTE_IDX]) == SIGN_POSITIVE) {
    return_code = ERROR;
  }
  if (!return_code && get_sign(src.bits[SIGN_BYTE_IDX]) != SIGN_POSITIVE) {
    if (*dst > MIN_INT) {
      *dst *= -1;
    }
  }

  return return_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL) {
    return ERROR;
  }
  long double mantissa = (unsigned int)src.bits[0] +
                         ldexpl((unsigned int)src.bits[1], 32) +
                         ldexpl((unsigned int)src.bits[2], 64);
  long double scale_normalizer = 1;
  int scale = get_scale(src.bits[SIGN_BYTE_IDX]);
  for (int i = 0; i < scale; i++) {
    scale_normalizer *= 10.0;
  }
  mantissa /= scale_normalizer;
  if (get_sign(src.bits[SIGN_BYTE_IDX]) == SIGN_NEGATIVE) {
    mantissa *= -1;
  }
  *dst = (float)mantissa;

  return SUCCESS;
}