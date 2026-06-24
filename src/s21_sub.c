#include "decimal_utils.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

    s21_decimal local_result;

    local_result.bits[0] = local_result.bits[1] = local_result.bits[2] = local_result.bits[3] = 0;

    if (get_scale(value_1.bits[SIGNIFICANT_BYTES]) != get_scale(value_2.bits[SIGNIFICANT_BYTES])) {
        align_scales(&value_1, &value_2);
    }
    set_scale(&local_result, get_scale(value_1.bits[SIGNIFICANT_BYTES]));

    int res = SUCCESS;

    if(get_sign(value_1.bits[SIGNIFICANT_BYTES]) != get_sign(value_2.bits[SIGNIFICANT_BYTES]) && !will_addition_overflow(value_1, value_2)) {
        set_sign(&local_result, get_sign(value_1.bits[SIGNIFICANT_BYTES]));
        s21_add_mantissas(value_1, value_2, &local_result);
    } else {
        if (will_addition_overflow(value_1, value_2)) {
            if (get_sign(value_1.bits[SIGNIFICANT_BYTES]) == SIGN_POSITIVE) {
                res = FAIL_TOO_LARGE;
            } else {
                res = FAIL_TOO_SMALL;
            }
        } else {
            if (compare_mantissas(value_1, value_2) == -1) {
                set_sign(&local_result, reverse_sign(get_sign(value_1.bits[SIGNIFICANT_BYTES])));
                s21_decimal temp;
                temp = value_1;
                value_1 = value_2;
                value_2 = temp;
            } else {
                set_sign(&local_result, get_sign(value_1.bits[SIGNIFICANT_BYTES]));
            }
            substract_mantissas(value_1, value_2, &local_result);
        }
    }

    if (res == SUCCESS) {
        *result = local_result;
    }

    return res;

}

int reverse_sign(int sign) {
    if (sign == SIGN_POSITIVE) {
        return SIGN_NEGATIVE;
    } else {
        return SIGN_POSITIVE;
    }
}