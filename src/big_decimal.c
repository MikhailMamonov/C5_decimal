#include "big_decimal.h"
#include "decimal_utils.h"
#include <stddef.h>

big_decimal assign(s21_decimal value) {
    big_decimal ret = {0};
    for (int i = 0; i < SIGNIFICANT_BYTES; i++) {
        ret.bits[i] = value.bits[i];
    }
    ret.bits[BIG_SIGNIFICANT_BYTES] = value.bits[SIGNIFICANT_BYTES];
    return ret;
}

s21_decimal assign_back(big_decimal value) {
    s21_decimal ret = {0};
    for (int i = 0; i < SIGNIFICANT_BYTES; i++) {
        ret.bits[i] = value.bits[i];
    }
    ret.bits[SIGNIFICANT_BYTES] = value.bits[BIG_SIGNIFICANT_BYTES];
    return ret;
}

int big_get_bit_val(big_decimal value, int idx) {
    int curr_byte = idx / BITS_IN_INT;
    int bit_num = idx % BITS_IN_INT;
    unsigned int bit_mask = 1 << bit_num;
    unsigned int bit = ((unsigned int)value.bits[curr_byte] & bit_mask) >> bit_num;

    return (int)bit;
}

int big_add_mantissas(big_decimal value_1, big_decimal value_2, big_decimal * result){
    unsigned long long carry = 0;

    for (int i=0;i<BIG_SIGNIFICANT_BYTES;i++){
        unsigned long long sum = (unsigned long long)(unsigned int)value_1.bits[i] + 
                                (unsigned long long)(unsigned int)value_2.bits[i] + carry;  
        result->bits[i] = (int)(sum & MAX_MASK);
        carry = sum >> BITS_IN_INT;
    }

    return (int)carry;
}

int big_shift_left_1_bit(big_decimal * value){
    unsigned int *b = (unsigned int *)value->bits;

    unsigned int last_overflow = 0;
    unsigned int overflow = 0;
    for (int i = 0; i < BIG_SIGNIFICANT_BYTES; i++) {
        overflow = (b[i] >> LAST_BIT) & SINGLE_BIT;
        b[i] <<= SINGLE_BIT;
        b[i] |= last_overflow;
        last_overflow = overflow;
    }

    return overflow;
}

int fits_in_decimal(big_decimal value) {
    int ret = 1;
    
    for (int i = SIGNIFICANT_BYTES; i < BIG_SIGNIFICANT_BYTES; i++) {
        if (value.bits[i] != 0) {
            ret = 0;
            break;
        }
    }

    return ret;
}

int big_div_by_10(big_decimal *num) {
    int remainder = 0;
    big_decimal copy = *num;
    for (int i = 0; i < BIG_SIGNIFICANT_BYTES; i++) {
        num->bits[i] = 0;
    }
    for (int i = BIG_ELDER_BIT; i >= 0; i--) {
            int curr_byte = i / BITS_IN_INT;
            int bit_num = i % BITS_IN_INT;
            unsigned int bit_mask = 1 << bit_num;
            unsigned int bit = ((unsigned int)copy.bits[curr_byte] & bit_mask) >> bit_num;
            remainder = remainder << 1;
            remainder += bit;
            if (remainder >= 10) {
                remainder -= 10;
                num->bits[curr_byte] = num->bits[curr_byte] | bit_mask;
            }
    }
    return remainder;
}

int big_add_one(big_decimal *value) {
    unsigned int *b = (unsigned int *)value->bits;
    int ret = 1;

    for (int i = 0; i < BIG_SIGNIFICANT_BYTES; i++) {
        if (b[i] < MAX_MASK) {
            b[i]++;
            ret = 0;
            break;
        } else {
            b[i] = 0;
        }
    }

    return ret;
}

int equals_zero(big_decimal value) {
    int ret = 1;

    for (int i = 0; i < BIG_SIGNIFICANT_BYTES; i++) {
        if (value.bits[i] != 0) {
            ret = 0;
            break;
        }
    }

    return ret;
}

int big_mul_by_10(big_decimal * value) {
    big_decimal mult = *value;
    for (int i = 0; i < BIG_SIGNIFICANT_BYTES; i++) {
        value->bits[i] = 0;
    }
    for (int i = 0; i < 3; i++) {
        big_shift_left_1_bit(&mult);
        if (!(i & 1)) {
            if (big_add_mantissas(*value, mult, value)) {
                return FAIL_TOO_LARGE;
            }
        }
    }
    return SUCCESS;
}

int big_decimal_is_zero(big_decimal value) {
    int is_zero = 1;
    for(int i = 0;i<BIG_SIGNIFICANT_BYTES;i++){
        if(value.bits[i]!=0){
            is_zero = 0;
            break;
        }
    }

    return is_zero;
}

int big_compare_mantissas(big_decimal value_1, big_decimal value_2) {
    int res = 0;
    for (int i = BIG_SIGNIFICANT_BYTES - 1; i >= 0; i--) {
        unsigned int a = (unsigned int) value_1.bits[i];
        unsigned int b = (unsigned int) value_2.bits[i];
        if (a > b) {
            res = 1; 
            break;
        }
        if (a < b) {
            res = -1; 
            break;
        }
    }
    return (int)res;
}

int big_is_less(big_decimal value_1, big_decimal value_2){
    int res = 0;

    int sign_value_1 = get_sign(value_1.bits[BIG_SIGNIFICANT_BYTES]);
    int sign_value_2 = get_sign(value_2.bits[BIG_SIGNIFICANT_BYTES]);
    
    if(sign_value_1!=sign_value_2){
        if(sign_value_1>sign_value_2){
            if(!(big_decimal_is_zero(value_1) && big_decimal_is_zero(value_2))){
                res = 1;
            }
        }
    }
    else{
        big_align_scales(&value_1, &value_2);
        int cmp = big_compare_mantissas(value_1, value_2);
        if(sign_value_1 == SIGN_POSITIVE){
            res = cmp<0;
        }
        else{
            res = cmp>0;
        }
    }
    
    return res;
}

int big_is_greater_or_equal(big_decimal value_1, big_decimal value_2){
    return !big_is_less(value_1, value_2);
}

int big_substract_mantissas(big_decimal value_1, big_decimal value_2, big_decimal *result) {
    unsigned long long borrow = 0;

    for (int i = 0; i < BIG_SIGNIFICANT_BYTES; i++) {
        unsigned int value_1_unsigned = (unsigned int)value_1.bits[i];
        unsigned int value_2_unsigned = (unsigned int)value_2.bits[i];
        unsigned long long diff = (unsigned long long)value_1_unsigned - value_2_unsigned - borrow;

        if (diff > 0xFFFFFFFFULL) {
            borrow = 1;
            result->bits[i] = (int)(unsigned int)(diff & MAX_MASK);
        } else {
            borrow = 0;
            result->bits[i] = (int)(unsigned int)diff;
        }
    }
    return (int)borrow;
}

int check_result(big_decimal *local_result, int *res_scale, int fail) {
    int exist_not_zero = 0, int_remainder = 0;
    while (*res_scale > MAX_SCALE || (!fits_in_decimal(*local_result) && *res_scale > 0)) {
        if (int_remainder > 0) {
            exist_not_zero = 1;
        }
        int_remainder = big_div_by_10(local_result);
        (*res_scale)--;
    }
    if (int_remainder > ROUND_THRESHOLD || (int_remainder == ROUND_THRESHOLD && exist_not_zero)) {
        big_add_one(local_result);
    } else if (int_remainder == ROUND_THRESHOLD) {
        if (local_result->bits[0] & 1) {
            big_add_one(local_result);
        }
    }
    if (!fits_in_decimal(*local_result)) {
        if (*res_scale > 0) {
            return check_result(local_result, res_scale, fail);
        } else {
            return fail;
        }
    } else {
        return SUCCESS;
    }
}

void big_align_scales(big_decimal * value_1, big_decimal * value_2){
    int scale_1 = get_scale(value_1->bits[BIG_SIGNIFICANT_BYTES]);
    int scale_2 = get_scale(value_2->bits[BIG_SIGNIFICANT_BYTES]);

    int isOverflow = 0;
    int existNonNull = 0;

    big_decimal backup_1 = *value_1;
    big_decimal backup_2 = *value_2;

    while (scale_1 != scale_2 && !isOverflow){
        big_decimal *target = (scale_1 < scale_2) ? value_1 : value_2;
        int *target_scale   = (scale_1 < scale_2) ? &scale_1 : &scale_2;

        if(big_mul_by_10(target)){
            isOverflow = 1;
        }
        else{
            (*target_scale)++;
            big_set_scale(target, *target_scale);
        }
    }

    if(isOverflow){
        *value_1 = backup_1;
        *value_2 = backup_2;
        scale_1 = get_scale(value_1->bits[BIG_SIGNIFICANT_BYTES]);
        scale_2 = get_scale(value_2->bits[BIG_SIGNIFICANT_BYTES]);

        int round_up = 0;
        big_decimal *source = NULL;

        int remainder = 0;
        while (scale_1 != scale_2){
            if (remainder != 0) {
                existNonNull = 1;
            }
            source = (scale_1 > scale_2) ? value_1 : value_2;
            int * source_scale = (scale_1 > scale_2) ? &scale_1 : &scale_2;
            
            remainder = big_div_by_10(source);
            
            (*source_scale)--;
            big_set_scale(source, *source_scale);
        }

        if(remainder>=ROUND_THRESHOLD){
             if(remainder > ROUND_THRESHOLD){
                round_up = 1;
            }
            else if(remainder == ROUND_THRESHOLD){
                if(existNonNull){
                    round_up= 1;    
                }
                //.5 clear digit -> Banking round
                else {
                    if ((unsigned int)source->bits[0] & 1) {
                        round_up = 1;
                    }     
                }
            }
            }

            if (round_up){
                big_add_one(source);
                int current_sign = get_sign(source->bits[3]);
                int current_scale = get_scale(source->bits[3]);
                big_set_sign(source, current_sign);
                big_set_scale(source,current_scale);
            }
    }
}

void big_set_sign(big_decimal *value, int sign) {
   int byte_value = SIGN_POSITIVE;
   if (sign == SIGN_NEGATIVE) { 
      byte_value = NEGATIVE_BYTE_VALUE;
   }
   value->bits[BIG_SIGNIFICANT_BYTES] = set_byte(value->bits[BIG_SIGNIFICANT_BYTES], SIGN_BYTE_IDX, byte_value);
}

void big_set_scale(big_decimal *value, int scale) {
   value->bits[BIG_SIGNIFICANT_BYTES] = set_byte(value->bits[BIG_SIGNIFICANT_BYTES], SCALE_BYTE_IDX, scale);
}
