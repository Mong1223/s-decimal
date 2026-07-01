#include "s21_help.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;

  s21_truncate(value, result);

  // Если число отрицательное и была дробная часть — уменьшаем на 1
  s21_decimal truncated;
  s21_truncate(value, &truncated);

  if (s21_get_sign(value) && !s21_is_equal(value, truncated)) {
    s21_decimal one = {{1, 0, 0, 0x80000000}};
    s21_add(*result, one, result);
  }

  s21_set_scale(result, 0);
  s21_set_sign(result, s21_get_sign(value));

  return 0;
}
