#include "s21_help.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;

  if (!(s21_is_zero(value_1) && s21_is_zero(value_2))) {
    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);

    if (sign1 && !sign2) {
      result = 1;
    } else if (!sign1 && sign2) {
      result = 0;
    } else {
      s21_align_scales(&value_1, &value_2);
      int cmp = s21_compare_mantissa(value_1, value_2);
      result = sign1 ? (cmp == 1) : (cmp == -1);
    }
  }

  return result;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  if (s21_is_zero(value_1) && s21_is_zero(value_2)) return 1;

  s21_align_scales(&value_1, &value_2);

  for (int i = 0; i < 3; i++) {
    if (value_1.bits[i] != value_2.bits[i]) return 0;
  }

  return s21_get_sign(value_1) == s21_get_sign(value_2);
}
int s21_is_less_or_equal(s21_decimal v1, s21_decimal value_2) {
  return s21_is_less(v1, value_2) || s21_is_equal(v1, value_2);
}

int s21_is_greater(s21_decimal v1, s21_decimal value_2) {
  return !s21_is_less_or_equal(v1, value_2);
}

int s21_is_greater_or_equal(s21_decimal v1, s21_decimal value_2) {
  return !s21_is_less(v1, value_2);
}

int s21_is_not_equal(s21_decimal v1, s21_decimal value_2) {
  return !s21_is_equal(v1, value_2);
}