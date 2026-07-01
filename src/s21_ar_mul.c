#include "s21_help.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *res) {
  int status = 0;

  int scale_sum = s21_get_scale(value_1) + s21_get_scale(value_2);
  if (s21_check_mul_args(res, scale_sum)) {
    status = 1;
  } else {
    s21_clear_bits(res);
    int sign = s21_get_sign(value_1) ^ s21_get_sign(value_2);
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);

    uint32_t result[6] = {0};
    s21_multiply_mantissas(&value_1, &value_2, result);

    if (result[3] || result[4] || result[5]) {
      if (s21_try_reduce_scale(result, &scale_sum)) {
        status = sign ? 2 : 1;
      }
    }

    if (status == 0) {
      s21_remove_trailing_zeros_mul(result, &scale_sum);
      res->bits[0] = result[0];
      res->bits[1] = result[1];
      res->bits[2] = result[2];
      s21_set_scale(res, scale_sum);
      s21_set_sign(res, sign);
    }
  }

  return status;
}
