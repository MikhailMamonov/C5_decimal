#include "decimal_utils.h"
#include "big_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    big_decimal local_value_1 = assign(value_1);
    big_decimal local_value_2 = assign(value_2);
    big_decimal local_result = {0};
    int result_status = SUCCESS;

    int result_sign = SIGN_POSITIVE;
    if (get_sign(value_1.bits[SIGN_BYTE_IDX]) != get_sign(value_2.bits[SIGN_BYTE_IDX])) {
        result_sign = SIGN_NEGATIVE;
    }
    int fail = FAIL_TOO_LARGE;
    if (result_sign == SIGN_NEGATIVE) {
        fail = FAIL_TOO_SMALL;
    }
    int result_scale = get_scale(value_1.bits[SIGN_BYTE_IDX]) + get_scale(value_2.bits[SIGN_BYTE_IDX]);

    int last_usable_bit = 0;
    for (int i = BIG_ELDER_BIT; i >= 0; i--) {
        int bit = big_get_bit_val(local_value_2, i);
        if (bit == 1) {
            last_usable_bit = i;
            break;
        }
    }

    for (int i = 0; i <= last_usable_bit; i++) {
        int bit = big_get_bit_val(local_value_2, i);
        if (bit && big_add_mantissas(local_result, local_value_1, &local_result)) {
            result_status = fail;
            break;
        }
        if (i < last_usable_bit && big_shift_left_1_bit(&local_value_1)) {
            result_status = fail;
            break;
        }
    }

    int exist_not_zero = 0, remainder = 0;
    while (result_scale > MAX_SCALE || (!fits_in_decimal(local_result) && result_scale > 0)) {
        if (remainder > 0) {
            exist_not_zero = 1;
        }
        remainder = big_div_by_10(&local_result);
        result_scale--;
    }
    if (remainder > ROUND_THRESHOLD || (remainder == ROUND_THRESHOLD && exist_not_zero)) {
        big_add_one(&local_result);
    } else if (remainder == ROUND_THRESHOLD) {
        if (local_result.bits[0] & 1) {
            big_add_one(&local_result);
        }
    }
    if (!fits_in_decimal(local_result)) {
        result_status = fail;
    }
    

    if (result_status == SUCCESS) {
        *result = assign_back(local_result);
        set_sign(result, result_sign);
        set_scale(result, result_scale);
    }

    return result_status;
}