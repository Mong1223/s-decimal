#include "s21_help.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;

  if (!result) {
    status = 1;
  } else {
    s21_clear_bits(result);
    s21_decimal_extended ext1 = {0}, ext2 = {0};
    s21_decimal_to_extended(&value_1, &ext1);
    s21_decimal_to_extended(&value_2, &ext2);
    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);
    s21_align_scales_extended(&ext1, &ext2);
    s21_decimal_extended ext_result = {0};

    if (sign1 == sign2) {
      s21_add_extended(&ext1, &ext2, &ext_result);
      s21_set_sign_extended(&ext_result, sign1);
    } else {
      int cmp = s21_compare_mantissa_extended(&ext1, &ext2);
      if (cmp == 0) {
        s21_clear_bits(result);
        status = 0;
      } else if (cmp > 0) {
        s21_subtract_extended(&ext1, &ext2, &ext_result);
        s21_set_sign_extended(&ext_result, sign1);
      } else {
        s21_subtract_extended(&ext2, &ext1, &ext_result);
        s21_set_sign_extended(&ext_result, sign2);
      }
    }

    if (status == 0 && s21_extended_to_decimal(&ext_result, result) != 0) {
      status = (s21_get_sign(value_1) == 0) ? 1 : 2;
    }
  }

  return status;
}
