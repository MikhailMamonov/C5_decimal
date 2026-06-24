#include "decimal_utils.h"

int get_sign(int service_bits){
    return (service_bits & SIGN_MASK) >> SIGN_BIT;
 }

int get_scale(int service_bits){
    return (service_bits & EXP_MASK) >> SCALE_BIT;
 }

 int compare_mantissas(s21_decimal value_1, s21_decimal value_2){
    (unsigned int)value_1.bits[0];
 }

 int s21_add_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal * result){
    unsigned long long carry = 0;

    for (int i=0;i<3;i++){
        unsigned long long sum = (unsigned long long)(unsigned int)value_1.bits[i] + 
                                (unsigned long long)(unsigned int)value_2.bits[i] + carry;  
        result->bits[i] = (int)(sum & MAX_MASK);
        carry = sum >> 32;
    }

    return (int)carry;
 }

 int substract_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    unsigned long long borrow = 0;

    for (int i = 0; i < SIGNIFICANT_BYTES; i++) {
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
   return;
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

void shift_left_1_bit(s21_decimal * value){
    int overflow_0_to_1 = (value->bits[0]>>LAST_BIT)&SINGLE_BIT; 
    int overflow_1_to_2 = (value->bits[1]>>LAST_BIT)&SINGLE_BIT;

    value->bits[0] <<=SINGLE_BIT;
    value->bits[1] <<=SINGLE_BIT;
    value->bits[2] <<=SINGLE_BIT;

    if(overflow_0_to_1){
        value->bits[1] |= overflow_0_to_1;
    }

    if(overflow_1_to_2){
        value->bits[2] |= overflow_1_to_2;
    }
}

void shift_left(s21_decimal * value, int bits_count){
   for (int i =0;i<bits_count;i++){
    shift_left_1_bit(value);
   }
}

int mul_by_10(s21_decimal * value){
    s21_decimal res_mul_by_8 = *value;
    s21_decimal res_mul_by_2 = *value;

    shift_left(&res_mul_by_8, 3);
    shift_left(&res_mul_by_2, 1);

    value->bits[0] = 0;
    value->bits[1] = 0;
    value->bits[2] = 0;

   return s21_add_mantissas(res_mul_by_8, res_mul_by_2, value);
}

void align_scales(s21_decimal * value_1, s21_decimal * value_2){
    int scale_1 = get_scale(value_1->bits[3]);
    int scale_2 = get_scale(value_2->bits[3]);

    while (scale_1!=scale_2){
        s21_decimal *target = (void *)0;
        s21_decimal *source = (void *)0;
        int * target_scale = (void *)0;
        int * source_scale = (void *)0;

        if(scale_1>scale_2){
            target_scale = &scale_1;
            source_scale = &scale_2;
            target = value_1;
            source = value_2;
        }
        else{
            target_scale = &scale_2;
            source_scale = &scale_1;
            target = value_2;
            source = value_1;
        }

        s21_decimal backup = *target;
        if(mul_by_10(target)){
            *target = backup;
            div_by_10(source);
            (*source_scale)--;
            set_scale(source, *source_scale);
        }
        else{
            (*target_scale)++;
            set_scale(target, *target_scale);
        }
    } 
}



