#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result){
    return 0;
}

int s21_round(s21_decimal value, s21_decimal *result){
    return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result){
    int current_scale = get_scale(value.bits[SIGN_BYTE_IDX]);
    for(int i=31-current_scale;i;i++){
        
    }
    return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result){
    if (!result)
    {
         return 1;
    }
    
    *result = value;
    set_sign(result, !get_sign(value.bits[SIGN_BYTE_IDX]));
    
    return 0; 
}
