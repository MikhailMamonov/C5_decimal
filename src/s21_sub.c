#include "decimal_utils.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (get_scale(value_1.bits[SIGNIFICANT_BYTES]) != get_scale(value_2.bits[SIGNIFICANT_BYTES])) {
        align_scales(&value_1, &value_2);
        set_scale(result, get_scale(value_1.bits[SIGNIFICANT_BYTES]));
    }

    int result = SUCCESS

    if(get_sign(value_1.bits[SIGNIFICANT_BYTES]) != get_sign(value_2.bits[SIGNIFICANT_BYTES]) && !will_addition_overflow(value_1, value_2)) {
        s21_add_mantissas(value_1, value_2, result);
    } else {
        if (get_sign(value_1.bits[SIGNIFICANT_BYTES]) == get_sign(value_2.bits[SIGNIFICANT_BYTES])) {
            if (compare_mantissas(value_1, value_2) == -1) {
                s21_decimal temp;
                temp = value_1;
                value_1 = value_2;
                value_2 = temp;
                set_sign(result, SIGN_NEGATIVE);
            }
            substract_mantissas(value_1, value_2, result);
        }
        if (will_addition_overflow(value_1, value_2)) {
            if (get_sign(value_1.bits[SIGNIFICANT_BYTES]) == SIGN_POSITIVE) {
                result = FAIL_TOO_LARGE;
            } else {
                result = FAIL_TOO_SMALL;
            }
        }
    }

    return result;

}