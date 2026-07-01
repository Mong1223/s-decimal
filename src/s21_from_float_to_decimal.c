#include "s21_help.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = 1;
  if (dst && !s21_is_invalid_float(src)) {
    status = 0;
    s21_clear_bits(dst);
    s21_decimal_extended ext = {0};

    int sign = signbit(src);
    if (sign) src = -src;

    double value = (double)src;
    int scale = 0;

    s21_normalize_float(&value, &scale);
    s21_round_double_to_significant(&value, &scale);
    s21_double_to_decimal_extended(value, &ext);
    s21_remove_trailing_zeros_fdecimal(&ext, &scale);

    dst->bits[0] = ext.bits[0];
    dst->bits[1] = ext.bits[1];
    dst->bits[2] = ext.bits[2];
    s21_set_scale(dst, scale);
    s21_set_sign(dst, sign);
  }
  return status;
}
