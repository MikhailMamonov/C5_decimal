#include "decimal_utils.h"
#include "big_decimal.h"

void division_cycle(big_decimal dividend, big_decimal divider, big_decimal *remainder, big_decimal *result, int last_bit);
int find_last_bit(big_decimal value_1);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    big_decimal local_value_1 = assign(value_1);
    big_decimal local_value_2 = assign(value_2);
    big_decimal local_result = {0};
    big_decimal remainder = {0};

    if (equals_zero(local_value_2)) {
        return FAIL_DIVIDE_ZERO;
    }

    int result_sign = SIGN_POSITIVE;
    if (get_sign(value_1.bits[SIGN_BYTE_IDX]) != get_sign(value_2.bits[SIGN_BYTE_IDX])) {
        result_sign = SIGN_NEGATIVE;
    }
    int res_scale = get_scale(value_1.bits[SIGNIFICANT_BYTES]) - get_scale(value_2.bits[SIGNIFICANT_BYTES]);

    while (res_scale < 0) {
        big_mul_by_10(&local_value_1);
        res_scale++;
    }
    int fail = FAIL_TOO_LARGE;
    if (result_sign == SIGN_NEGATIVE) {
        fail = FAIL_TOO_SMALL;
    }
    int result_status = SUCCESS;

    int last_usable_bit = find_last_bit(local_value_1);

    division_cycle(local_value_1, local_value_2, &remainder, &local_result, last_usable_bit);

    while (!equals_zero(remainder) && res_scale <= MAX_SCALE) {
        big_mul_by_10(&local_result);
        big_mul_by_10(&remainder);
        local_value_1 = remainder;
        last_usable_bit = find_last_bit(local_value_1);
        for (int i = 0; i < BIG_SIGNIFICANT_BYTES; i++) {
            remainder.bits[i] = 0;
        }
        res_scale++;
        big_decimal digit_result = {0};

        division_cycle(local_value_1, local_value_2, &remainder, &digit_result, BIG_ELDER_BIT);
        big_add_mantissas(local_result, digit_result, &local_result);
    }

    result_status = check_result(&local_result, &res_scale, fail);

    if (result_status == SUCCESS) {
        *result = assign_back(local_result);
        set_sign(result, result_sign);
        set_scale(result, res_scale);
    }

    return result_status;
}

void division_cycle(big_decimal dividend, big_decimal divider, big_decimal *remainder, big_decimal *result, int last_bit) {
    for (int i = last_bit; i >= 0; i--) {
        int bit = big_get_bit_val(dividend, i);
        big_shift_left_1_bit(remainder);
        if (bit) {
            big_add_one(remainder);
        }
        if (big_is_greater_or_equal(*remainder, divider)) {
            big_substract_mantissas(*remainder, divider, remainder);
            int curr_byte = i / BITS_IN_INT;
            int bit_num = i % BITS_IN_INT;
            unsigned int bit_mask = 1 << bit_num;
            result->bits[curr_byte] = result->bits[curr_byte] | bit_mask;
        }
    }
}

int find_last_bit(big_decimal value_1) {
    int last_usable_bit = 0;
    for (int i = BIG_ELDER_BIT; i >= 0; i--) {
        int bit = big_get_bit_val(value_1, i);
        if (bit == 1) {
            last_usable_bit = i;
            break;
        }
    }
    return last_usable_bit;
}
