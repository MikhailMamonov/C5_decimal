#define COUNT_OF_BITS 4
#define SIGNIFICANT_BYTES 3
#define SIGN_NEGATIVE 1
#define SIGN_POSITIVE 0
#define SUCCESS 0
#define FAIL_TOO_LARGE 1
#define FAIL_TOO_SMALL 2
#define FAIL_DIVIDE_ZERO 3
#define EXP_MASK 0x00FF0000
#define SIGN_MASK 0x80000000
#define SIGN_BIT 31
#define MAX_MASK 0xFFFFFFFF
#define MAX_UNSIGNED_LONG 0xFFFFFFFFULL
#define SCALE_BIT 16 


typedef struct 
{
    int bits[4];
} s21_decimal;



int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
