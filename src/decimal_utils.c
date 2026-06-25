#include "decimal_utils.h"
#include <math.h>

int get_sign(int service_bits){
    return (service_bits & SIGN_MASK) >> SIGN_BIT;
 }

int get_scale(int service_bits){
    return (service_bits & EXP_MASK) >> SCALE_BIT;
 }

 int compare_mantissas(s21_decimal value_1, s21_decimal value_2) {
    int res = 0;
    for (int i = SIGNIFICANT_BYTES - 1; i >= 0; i--) {
      unsigned int a = (unsigned int) value_1.bits[i];
      unsigned int b = (unsigned int) value_2.bits[i];
      if (a > b) {
            res = SIGN_POSITIVE; 
            break;
        }
        if (a < b) {
            res = SIGN_NEGATIVE; 
            break;
        }
    }
    return (int)res;
 }

 int substract_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    unsigned long long borrow = 0;

    for (int i = 0; i < SIGNIFICANT_BYTES; i++) {
        unsigned int value_1_unsigned = (unsigned int)value_1.bits[i];
        unsigned int value_2_unsigned = (unsigned int)value_2.bits[i];
        unsigned long long diff = (unsigned long long)value_1_unsigned - value_2_unsigned - borrow;

        if (diff > MAX_UNSIGNED_LONG) {
            borrow = 1;
            result->bits[i] = (int)(unsigned int)(diff & MAX_MASK);
        } else {
            borrow = 0;
            result->bits[i] = (int)(unsigned int)diff;
        }
    }
    return (int)borrow;
}


 int will_addition_overflow(s21_decimal value_1, s21_decimal value_2){
    unsigned int stock0 = MAX_MASK - (unsigned int)value_1.bits[0]; 
    unsigned int stock1 = MAX_MASK - (unsigned int)value_1.bits[1];
    unsigned int stock2 = MAX_MASK - (unsigned int)value_1.bits[2];

    unsigned int value_2_bits_0 = (unsigned int)value_2.bits[0];
    unsigned int value_2_bits_1 = (unsigned int)value_2.bits[1];
    unsigned int value_2_bits_2 = (unsigned int)value_2.bits[2];

    
    if(stock2 < value_2_bits_2 ||
     (stock2==value_2_bits_2 && stock1<value_2_bits_1) || 
     (stock2==value_2_bits_2&&stock1==value_2_bits_1 && stock0<value_2_bits_0 )){
        return 1;
    }
    return 0;
 }

void set_sign(s21_decimal *value, int sign) {
   int byte_value = SIGN_POSITIVE;
   if (sign == SIGN_NEGATIVE) { 
      byte_value = NEGATIVE_BYTE_VALUE;
   }
   value->bits[SIGNIFICANT_BYTES] = set_byte(value->bits[SIGNIFICANT_BYTES], SIGN_BYTE_IDX, byte_value);
}

void set_scale(s21_decimal *value, int scale) {
   value->bits[SIGNIFICANT_BYTES] = set_byte(value->bits[SIGNIFICANT_BYTES], SCALE_BYTE_IDX, scale);
}

int set_byte(int num, int byte_index, unsigned char new_byte) {
    int shift = byte_index * BITS_IN_BYTE;

    int clear_mask = ~(0xFF << shift);

    num = num & clear_mask;

    num = num | (new_byte << shift);

    return num;
}

int div_by_10(s21_decimal *num) {
    int remainder = 0;
    s21_decimal copy = *num;
    for (int i = 0; i < SIGNIFICANT_BYTES; i++) {
        num->bits[i] = 0;
    }
    for (int i = ELDER_BIT; i >= 0; i--) {
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
    /*if (remainder > 5) {
        answer = add_one(answer);
    } else if (remainder == 5) {
        int even = 0;
        int first_bit_mask = 1;
        if (answer.bits[0] & first_bit_mask) {
            answer = add_one(answer);
        }
    }*/
    return remainder;
}

s21_decimal add_one(s21_decimal num) {
    s21_decimal one;
    one.bits[0] = 1;
    for (int i = 1 ; i <= SIGNIFICANT_BYTES; i++) {
        one.bits[i] = 0;
    }
    set_scale(&one, 0);
    set_sign(&one, SIGN_POSITIVE);
    s21_add(num, one, &num);
    return num;
}

