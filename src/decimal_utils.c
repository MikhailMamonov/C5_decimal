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