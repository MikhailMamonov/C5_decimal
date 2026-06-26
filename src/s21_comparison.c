#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2){
    int res = 0;

    int sign_value_1 = get_sign(value_1.bits[3]);
    int sign_value_2 = get_sign(value_2.bits[3]);
    
    if(sign_value_1!=sign_value_2){
        if(sign_value_1>sign_value_2){
            if(!decimal_is_zero(value_1) || !decimal_is_zero(value_2)){
                res = 1;
            }
        }
    }
    else{
        align_scales(&value_1, &value_2);
        int cmp = compare_mantissas(value_1, value_2);
        if(sign_value_1 == SIGN_POSITIVE){
            if(cmp == SIGN_NEGATIVE){
                res = 1;
            }
        }
        else{
            if(cmp == SIGN_POSITIVE)
            {
                res = 1;
            }
        }
    }
    
    return (int)res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
