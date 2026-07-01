#include "s21_help.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *res) {
  int status = 0;

  if (!res) {
    status = 1;
  } else if (s21_is_zero(value_2)) {
    status = 3;
  } else {
    s21_clear_bits(res);
    int result_sign = 0;
    s21_prepare_division(&value_1, &value_2, &result_sign);

    s21_decimal_extended ext1 = {0}, ext2 = {0}, quotient = {0},
                         remainder = {0};
    s21_decimal_to_extended(&value_1, &ext1);
    s21_decimal_to_extended(&value_2, &ext2);
    s21_align_scales_extended(&ext1, &ext2);

    int scale1 = (ext1.bits[6] >> 16) & 0xFF;
    int scale2 = (ext2.bits[6] >> 16) & 0xFF;
    int result_scale = 0;
    int precision_added = 0;

    s21_increase_precision(&ext1, &ext2, &result_scale, &precision_added,
                           scale1, scale2);
    s21_binary_division_extended(&ext1, &ext2, &quotient, &remainder);

    if (precision_added > 0) {
      s21_apply_bank_rounding(&quotient, &remainder, &ext2);
    }

    if (result_scale < 0) result_scale = 0;
    if (result_scale > 28) result_scale = 28;

    quotient.bits[6] = (result_scale << 16) | (result_sign ? 0x80000000 : 0);

    if (s21_extended_to_decimal(&quotient, res) != 0) {
      status = result_sign ? 2 : 1;
    }
  }

  return status;
}
