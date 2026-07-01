#include "s21_help.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int err = 1;
  if (!dst) {
    return err;
  }
  if (s21_is_valid_int(src)) {
    s21_clear_bits(dst);
    s21_set_sign(dst, src < 0 ? 1 : 0);
    dst->bits[0] = (unsigned int)(src < 0 ? -src : src);
    err = 0;
  }
  return err;
}