#include "test_s21_common.h"

#include <stdio.h>

#include "../s21_decimal.h"

int compare_decimal(const s21_decimal a, const s21_decimal b) {
  for (int i = 0; i < COUNT_OF_BITS; i++) {
    if (a.bits[i] != b.bits[i]) {
      return 0;
    }
  }

  return 1;
}

void decimal_to_string(const s21_decimal a, char* buffer) {
  sprintf(buffer, "bits[3]:%08X bits[2]:%08X bits[1]:%08X bits[0]:%08X",
          (unsigned int)a.bits[SIGN_BYTE_IDX], (unsigned int)a.bits[2],
          (unsigned int)a.bits[1], (unsigned int)a.bits[0]);
}
