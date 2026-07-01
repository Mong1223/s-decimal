#include "s21_help.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;

  *result = value;
  int scale = s21_get_scale(value);

  for (int i = 0; i < scale; i++) s21_div_10(result);

  s21_set_scale(result, 0);
  s21_set_sign(result, s21_get_sign(value));

  return 0;
}