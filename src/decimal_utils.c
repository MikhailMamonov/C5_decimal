#include "decimal_utils.h"
int get_sign(int service_bits){
    return (service_bits & SIGN_MASK) >> SIGN_BIT;
 }

int get_scale(int service_bits){
    return (service_bits & EXP_MASK) >> SCALE_BIT;
 }

 int compare_mantissas(s21_decimal value_1, s21_decimal value_2){
    (unsigned int)value_1.bits[0]
 }

 void s21_add_mantissas(s21_decimal value_1, s21_decimal value_2, s21_decimal * result){
    unsigned long long carry = 0;

    for (int i=0;i<3;i++){
        unsigned long long sum = value_1.bits[i]+value_2.bits[i]+ carry;  
        result->bits[i] = (int)(sum & MAX_MASK);
        carry = sum >> 32;
    }

 }