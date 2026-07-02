#include "decimal_utils.h"

int reverse_sign(int sign);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) {
    return FAIL_TOO_LARGE;
  }

  s21_decimal local_result = {0};

  if (get_scale(value_1.bits[SIGN_BYTE_IDX]) !=
      get_scale(value_2.bits[SIGN_BYTE_IDX])) {
    align_scales(&value_1, &value_2);
  }
  set_scale(&local_result, get_scale(value_1.bits[SIGN_BYTE_IDX]));

  int result_status = SUCCESS;

  int will_addition_overflow =
      s21_add_mantissas(value_1, value_2, &local_result);

  if (get_sign(value_1.bits[SIGN_BYTE_IDX]) !=
          get_sign(value_2.bits[SIGN_BYTE_IDX]) &&
      !will_addition_overflow) {
    set_sign(&local_result, get_sign(value_1.bits[SIGN_BYTE_IDX]));
  } else {
    if (get_sign(value_1.bits[SIGN_BYTE_IDX]) !=
            get_sign(value_2.bits[SIGN_BYTE_IDX]) &&
        will_addition_overflow) {
      if (get_sign(value_1.bits[SIGN_BYTE_IDX]) == SIGN_POSITIVE) {
        result_status = FAIL_TOO_LARGE;
      } else {
        result_status = FAIL_TOO_SMALL;
      }
    } else {
      if (compare_mantissas(value_1, value_2) == -1) {
        set_sign(&local_result,
                 reverse_sign(get_sign(value_1.bits[SIGN_BYTE_IDX])));
        s21_decimal temp;
        temp = value_1;
        value_1 = value_2;
        value_2 = temp;
      } else {
        set_sign(&local_result, get_sign(value_1.bits[SIGN_BYTE_IDX]));
      }
      substract_mantissas(value_1, value_2, &local_result);
    }
  }

  if (result_status == SUCCESS) {
    *result = local_result;
  }
  return result_status;
}

int reverse_sign(int sign) {
  if (sign == SIGN_POSITIVE) {
    return SIGN_NEGATIVE;
  } else {
    return SIGN_POSITIVE;
  }
}