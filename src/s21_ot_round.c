#include "s21_help.h"

// округляет decimal до ближайшего целого
int s21_round(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;

  int sign = s21_get_sign(value);
  int scale = s21_get_scale(value);

  if (scale == 0) {
    *result = value;
    return 0;
  }

  s21_decimal rounded = value;
  s21_decimal half = {{5, 0, 0, 0}};
  s21_set_scale(&half, 1);

  if (sign)
    s21_sub(rounded, half, &rounded);
  else
    s21_add(rounded, half, &rounded);

  s21_truncate(rounded, result);
  s21_set_sign(result, sign);
  s21_set_scale(result, 0);

  return 0;
}
