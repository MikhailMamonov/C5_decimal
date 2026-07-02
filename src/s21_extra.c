#include "s21_decimal.h"
#include "stddef.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (result == NULL) {
    return ERROR;
  }

  int sign = get_sign(value.bits[SIGN_BYTE_IDX]);
  int scale = get_scale(value.bits[SIGN_BYTE_IDX]);

  if (scale > MAX_SCALE) {
    return ERROR;
  }

  *result = value;
  if (sign == SIGN_NEGATIVE || scale > 0) {
    if (!sign) {
      s21_truncate(value, result);
    } else {
      int has_fraction = 0;

      set_sign(result, 0);
      set_scale(result, 0);

      while (scale > 0) {
        int remainder = div_by_10(result);
        if (remainder != 0) {
          has_fraction = 1;  // Если нашли хоть одну цифру > 0, фиксируем это
        }
        scale--;
      }

      if (has_fraction && add_one(result) > 0) {
        // Переполнение
        return ERROR;
      }
    }
    set_sign(result, sign);
    set_scale(result, 0);
  }

  return SUCCESS;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (result == NULL) {
    return ERROR;
  }

  int sign = get_sign(value.bits[SIGN_BYTE_IDX]);
  int scale = get_scale(value.bits[SIGN_BYTE_IDX]);

  if (scale < 0 || scale > MAX_SCALE) {
    return ERROR;
  }

  *result = value;

  // Полностью очищаем управляющий байт перед делением
  set_sign(result, 0);
  set_scale(result, 0);

  int existNonNull = 0;
  int remainder = 0;

  while (scale > 0) {
    remainder = div_by_10(result);

    if (scale > 1 && remainder != 0) {
      existNonNull = 1;
    }

    scale--;
  }

  int round_up = 0;

  if (remainder > 5) {
    round_up = 1;
  } else if (remainder == 5) {
    if (existNonNull) {
      round_up = 1;
    } else {
      if ((unsigned int)result->bits[0] & 1) {
        round_up = 1;
      }
    }
  }

  if (round_up && add_one(result) > 0) {
    // Переполнение
    return ERROR;
  }

  set_sign(result, sign);
  set_scale(result, 0);

  return SUCCESS;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) {
    return ERROR;
  }

  int current_scale = get_scale(value.bits[SIGN_BYTE_IDX]);

  if (current_scale > MAX_SCALE) {
    return ERROR;  // ERROR_INVALID_SCALE
  }

  *result = value;

  for (int i = current_scale; i > 0; i--) {
    div_by_10(result);
  }

  set_scale(result, 0);
  return SUCCESS;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) {
    return ERROR;
  }

  *result = value;
  if (decimal_is_zero(value)) {
    // Ноль всегда положительный
    set_sign(result, SIGN_POSITIVE);
  } else {
    set_sign(result, !get_sign(value.bits[SIGN_BYTE_IDX]));
  }

  return SUCCESS;
}
