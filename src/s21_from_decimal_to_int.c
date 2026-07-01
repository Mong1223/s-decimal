#include "s21_help.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int err = 1;
  if (!dst || !s21_is_valid_decimal(src)) {
    return 1;
  }
  int scale = s21_get_scale(src);
  long long value = 0;
  for (int i = 0; i < 96; i++) {
    if (s21_get_bit(src, i)) {
      value += pow(2, i);
    }
  }
  if (scale >= 0 && scale <= 28) {
    value *= pow(10, -scale);
    if (s21_get_sign(src)) {
      value = -value;
    }
    if (s21_is_valid_int(value)) {
      *dst = (int)value;
      err = 0;
    }
  } else {
    *dst = 0;
  }
  return err;
}