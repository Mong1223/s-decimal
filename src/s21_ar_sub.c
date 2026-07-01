#include "s21_help.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal negated = {0};
  int err = s21_negate(value_2, &negated);
  if (!err) err = s21_add(value_1, negated, result);

  return err;
}