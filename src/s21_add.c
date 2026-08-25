#include "decimal_utils.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) {
    return FAIL_TOO_LARGE;
  }

  align_scales(&value_1, &value_2);

  int sign_value_1 = get_sign(value_1.bits[SIGN_BYTE_IDX]);
  int sign_value_2 = get_sign(value_2.bits[SIGN_BYTE_IDX]);
  int final_scale = get_scale(value_1.bits[SIGN_BYTE_IDX]);

  result->bits[0] = result->bits[1] = result->bits[2] =
      result->bits[SIGN_BYTE_IDX] = 0;
  s21_decimal *backup = result;

  int return_status = SUCCESS;

  if (sign_value_1 == sign_value_2) {
    int overflow = s21_add_mantissas(value_1, value_2, result);
    if (overflow) {
      result = backup;
      if (sign_value_1) {
        return_status = FAIL_TOO_SMALL;
      } else {
        return_status = FAIL_TOO_LARGE;
      }
    } else {
      set_sign(result, sign_value_1);
    }
  } else {
    int cmp = compare_mantissas(value_1, value_2);
    if (cmp > 0) {
      substract_mantissas(value_1, value_2, result);
      set_sign(result, sign_value_1);
    } else if (cmp < 0) {
      substract_mantissas(value_2, value_1, result);
      set_sign(result, sign_value_2);
    } else {
      return_status = SUCCESS;
    }
  }

  if (return_status == SUCCESS) {
    set_scale(result, final_scale);
  }

  return return_status;
}