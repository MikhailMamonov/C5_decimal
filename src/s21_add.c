#include "s21_decimal.h";



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

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
    int sign_value_1 = get_sign(value_1.bits[3]);
    int sign_value_2 = get_sign(value_2.bits[3]);
    int scale_value_1 = get_scale(value_1.bits[3]);
    int scale_value_2 = get_scale(value_2.bits[3]);

    if (scale_value_1 != scale_value_2){
        align_scales(&value_1, &value_2);
    }


    int return_value = SUCCESS;
    
    if(sign_value_1!=sign_value_2 || !will_addition_overflow(value_1, value_2))
    {
        if(sign_value_1==sign_value_2 ){
            s21_add_mantissas(value_1, value_2, result);
    }
    else{

    }
    } 
    else{
        if(sign_value_1)
        {
            return_value = FAIL_TOO_SMALL;
        }
        else
        {
            return_value = FAIL_TOO_LARGE;
        }
    }

    return return_value;
}