#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define COUNT_OF_BITS 4
#define SIGNIFICANT_BYTES 3
#define SUCCESS 0
#define SIGN_NEGATIVE 1
#define SIGN_POSITIVE 0
#define FAIL_TOO_LARGE 1
#define FAIL_TOO_SMALL 2
#define FAIL_DIVIDE_ZERO 3
#define EXP_MASK 0x00FF0000
#define SIGN_MASK 0x80000000
#define SIGN_BIT 31
#define MAX_MASK 0xFFFFFFFF
#define MAX_UNSIGNED_LONG 0xFFFFFFFFULL
#define SCALE_BIT 16 
#define NEGATIVE_BYTE_VALUE 128
#define SCALE_BYTE_IDX 2
#define BITS_IN_BYTE 8
#define ELDER_BIT 95
#define SIGN_BYTE_IDX 3
#define BITS_IN_INT 32
#define ROUND_THRESHOLD 5
#define MAX_SCALE 28
#define LAST_BIT 31
#define SINGLE_BIT 1

typedef struct 
{
    int bits[4];
} s21_decimal;


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif