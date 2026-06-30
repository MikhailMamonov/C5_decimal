#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2){
    int res = 0;

    int sign_value_1 = get_sign(value_1.bits[SIGN_BYTE_IDX]);
    int sign_value_2 = get_sign(value_2.bits[SIGN_BYTE_IDX]);
    
    if(sign_value_1!=sign_value_2){
        if(sign_value_1>sign_value_2){
            if(!(decimal_is_zero(value_1) && decimal_is_zero(value_2))){
                res = 1;
            }
        }
    }
    else{
        align_scales(&value_1, &value_2);
        int cmp = compare_mantissas(value_1, value_2);
        if(sign_value_1 == SIGN_POSITIVE){
            res = cmp<0;
        }
        else{
            res = cmp>0;
        }
    }
    
    return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2){
    return s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2)
{
    return s21_is_less(value_2, value_1);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2){
    return !s21_is_less(value_1, value_2);
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2){
    int res = 0;

    int sign_value_1 = get_sign(value_1.bits[SIGN_BYTE_IDX]);
    int sign_value_2 = get_sign(value_2.bits[SIGN_BYTE_IDX]);

    if(sign_value_1!=sign_value_2){
        res = decimal_is_zero(value_1) && decimal_is_zero(value_2);
    }
    else{
        align_scales(&value_1, &value_2);
        res = compare_mantissas(value_1, value_2) == 0;
    }

    return res;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2){
    return !s21_is_equal(value_1, value_2);
}
