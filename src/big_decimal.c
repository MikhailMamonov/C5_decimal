#include "big_decimal.h"

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
