#include "convertors.h"
#include "decimal_utils.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "ctype.h"

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
    if (dst == NULL || fabs(src) == INFINITY) {
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