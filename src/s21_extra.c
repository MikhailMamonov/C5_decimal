#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result){
    if (result == (void*)0) {
        return 1; 
    }

    int sign = get_sign(value.bits[SIGN_BYTE_IDX]);
    int scale = get_scale(value.bits[SIGN_BYTE_IDX]);
    
    if(scale>28){
        return 1;
    }

    *result = value;
    if(sign>0 || scale>0){
        if(!sign){
            s21_truncate(value, result);
        }
        else{
            int has_fraction = 0;

            set_sign(result, 0);
            set_scale(result, 0);

            while (scale > 0){
                int remainder = div_by_10(result);
                if (remainder != 0) {
                    has_fraction = 1; // Если нашли хоть одну цифру > 0, фиксируем это
                }
                scale--;
            }
            
            if(has_fraction){
                s21_decimal one = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
                s21_decimal temp = *result;
                
                s21_add_mantissas(*result, one, &temp);
                
                *result = temp;
        }
        set_sign(result, sign);
        set_scale(result,0);
        }
    }
    return 0;
}

  int s21_round(s21_decimal value, s21_decimal *result) {
    if (result == (void*)0) {
        return 1; 
    }

    int sign = get_sign(value.bits[SIGN_BYTE_IDX]);
    int scale = get_scale(value.bits[SIGN_BYTE_IDX]);
    
    if (scale < 0 || scale > 28) {
        return 1;
    }
    
    *result = value;

    // Полностью очищаем управляющий байт перед делением
    set_sign(result, 0);
    set_scale(result, 0);

    int first_digit = 0;  
    int existNonNull = 0;

    while (scale > 0) {
        int remainder = div_by_10(result);
        
        // Если это САМЫЙ ПОСЛЕДНИЙ шаг деления (первая цифра после запятой)
        if (scale == 1) {
            first_digit = remainder;
        } 
        // Если это любой другой шаг деления, и мы встретили НЕ ноль
        if (scale > 1 && remainder != 0) {
            existNonNull = 1;
        }
        
        scale--;
    }

    int round_up = 0;

    // Банковское округление
    if (first_digit > 5) {
        round_up = 1;
    } else if (first_digit == 5) {
        // Если после пятерки были еще цифры (как .5567) -> округляем всегда вверх
        if (existNonNull) {
            round_up = 1;
        } else {
            // Если была чистая половина (как .5000) -> округляем к ближайшему четному
            if ((unsigned int)result->bits[0] & 1) {
                round_up = 1;
            }
        }
    }

    // Прибавление единицы без вызова сторонних функций сложения
    if (round_up) {
        unsigned long long carry = 1;
        for (int i = 0; i < 3; i++) {
            unsigned long long sum = (unsigned long long)(unsigned int)result->bits[i] + carry;
            result->bits[i] = (int)(sum & 0xFFFFFFFF);
            carry = sum >> 32;
        }
        if (carry != 0) {
            return 1;  // Переполнение мантиссы
        }
    }

    // Возвращаем исходный знак
    set_sign(result, sign);
    set_scale(result, 0);

    return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result){
     if (result == (void*)0) {
        return 1; 
    }

    int current_scale = get_scale(value.bits[SIGN_BYTE_IDX]);

    if (current_scale > MAX_SCALE) {
        return 1;  // ERROR_INVALID_SCALE
    }

    *result = value;

    for(int i=current_scale;i>0;i--){
        div_by_10(result);

    }

    set_scale(result, 0);
    return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result){
    if (!result)
    {
         return 1;
    }
    
    *result = value;
    if(decimal_is_zero(value)){
        // Ноль всегда положительный
        result->bits[3] = 0;
    }
    else{
        set_sign(result, !get_sign(value.bits[SIGN_BYTE_IDX]));    
    }

    return 0; 
}
