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
