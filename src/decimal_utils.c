#include "decimal_utils.h"
#include <stddef.h>

int set_byte(int num, int byte_index, unsigned char new_byte);

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

 int s21_add_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal * result){
    unsigned long long carry = 0;

    for (int i=0;i<SIGNIFICANT_BYTES;i++){
        unsigned long long sum = (unsigned long long)(unsigned int)value_1.bits[i] + 
                                (unsigned long long)(unsigned int)value_2.bits[i] + carry;  
        result->bits[i] = (int)(sum & MAX_MASK);
        carry = sum >> BITS_IN_INT;
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

int shift_left_1_bit(s21_decimal * value){
    unsigned int *b = (unsigned int *)value->bits;
    
    int overflow_0_to_1 = (b[0]>>LAST_BIT)&SINGLE_BIT; 
    int overflow_1_to_2 = (b[1]>>LAST_BIT)&SINGLE_BIT;
    int overflow_all = (b[2]>> LAST_BIT)&SINGLE_BIT;
    b[0] <<=SINGLE_BIT;
    b[1] <<=SINGLE_BIT;
    b[2] <<=SINGLE_BIT;

    if(overflow_0_to_1){
        b[1] |= 1u;
    }

    if(overflow_1_to_2){
        b[2]|= 1u;
    }

    return overflow_all;
}

int shift_left(s21_decimal * value, int bits_count){
   for (int i =0;i<bits_count;i++){
    if(shift_left_1_bit(value)){
        return 1;
    }
   }

   return 0;
}

int mul_by_10(s21_decimal * value){
    s21_decimal res_mul_by_8 = *value;
    s21_decimal res_mul_by_2 = *value;

    if(shift_left(&res_mul_by_8, 3) || shift_left(&res_mul_by_2, 1)){
        return 1;
    }

    value->bits[0] = 0;
    value->bits[1] = 0;
    value->bits[2] = 0;

   return s21_add_mantissas(res_mul_by_8, res_mul_by_2, value);
}

void align_scales(s21_decimal * value_1, s21_decimal * value_2){
    int scale_1 = get_scale(value_1->bits[3]);
    int scale_2 = get_scale(value_2->bits[3]);

    int isOverflow = 0;
    int existNonNull = 0;

    s21_decimal backup_1 = *value_1;
    s21_decimal backup_2 = *value_2;

    while (scale_1 != scale_2 && !isOverflow){
        s21_decimal *target = (scale_1 < scale_2) ? value_1 : value_2;
        int *target_scale   = (scale_1 < scale_2) ? &scale_1 : &scale_2;

        if(mul_by_10(target)){
            isOverflow = 1;
        }
        else{
            (*target_scale)++;
            set_scale(target, *target_scale);
        }
    }

    if(isOverflow){
        *value_1 = backup_1;
        *value_2 = backup_2;
        scale_1 = get_scale(value_1->bits[3]);
        scale_2 = get_scale(value_2->bits[3]);

        int round_up = 0;
        s21_decimal *source = NULL;

        int remainder = 0;
        while (scale_1 != scale_2){
            if (remainder != 0) {
                existNonNull = 1;
            }
            source = (scale_1 > scale_2) ? value_1 : value_2;
            int * source_scale = (scale_1 > scale_2) ? &scale_1 : &scale_2;
            
            remainder = div_by_10(source);
            
            (*source_scale)--;
            set_scale(source, *source_scale);
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
                s21_decimal one = {0};
                one.bits[0] = 1;
                s21_decimal temp_res ={0};
                set_sign(source, 0);
                set_scale(source,0);

                s21_add_mantissas(*source, one, &temp_res);

                int current_sign = get_sign(source->bits[3]);
                int current_scale = get_scale(source->bits[3]);

                *source = temp_res;

                set_sign(source, current_sign);
                set_scale(source,current_scale);
            }
    }
}


int get_bit_val(s21_decimal value, int idx) {
    int curr_byte = idx / BITS_IN_INT;
    int bit_num = idx % BITS_IN_INT;
    unsigned int bit_mask = 1 << bit_num;
    unsigned int bit = ((unsigned int)value.bits[curr_byte] & bit_mask) >> bit_num;

    return (int)bit;
}

