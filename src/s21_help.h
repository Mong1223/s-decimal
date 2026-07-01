#ifndef S21_DECIMAL_EXTENDED_H
#define S21_DECIMAL_EXTENDED_H
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_decimal.h"
#define VALIDATION_MASK 0x7F00FFFFu
#define MAX_DECIMAL_FLOAT 79228162514264337593543950335.0f
#define MIN_DECIMAL_FLOAT 1e-28f

typedef struct {
  unsigned int bits[7];
} s21_decimal_extended;

// simples
s21_decimal s21_make_decimal(int lo, int mid, int hi, int sign, int scale);
int s21_is_zero(s21_decimal value);
void s21_clear_bits(s21_decimal *value);
int s21_get_bit(s21_decimal value, int position);
int s21_set_bit(s21_decimal *value, int position, int bit);
int s21_get_sign(s21_decimal value);
int s21_set_sign(s21_decimal *value, int sign);
int s21_get_scale(s21_decimal value);
int s21_set_scale(s21_decimal *value, int scale);
int s21_is_valid_position(int position);
int s21_is_valid_scale(int scale);
int s21_is_valid_decimal(s21_decimal src);
int s21_is_valid_int(double src);

// comparison
int s21_compare_mantissa(s21_decimal value_1, s21_decimal value_2);
int s21_align_scales(s21_decimal *value_1, s21_decimal *value_2);
void s21_multiply_by_10(s21_decimal *value);
int s21_can_multiply_by_10(s21_decimal value);
int s21_div_10_t(s21_decimal *value);

// truncate
void s21_div_10(s21_decimal *value);

// add
void s21_decimal_to_extended(s21_decimal *src, s21_decimal_extended *dst);
void s21_remove_trailing_zeros_extended(s21_decimal_extended *val);
int s21_extended_to_decimal(s21_decimal_extended *src, s21_decimal *dst);
void s21_add_extended(s21_decimal_extended *a, s21_decimal_extended *b,
                      s21_decimal_extended *res);
void s21_subtract_extended(s21_decimal_extended *a, s21_decimal_extended *b,
                           s21_decimal_extended *res);
void s21_multiply_by_10_extended(s21_decimal_extended *val);
int s21_can_multiply_by_10_extended(s21_decimal_extended val);
void s21_align_scales_extended(s21_decimal_extended *a,
                               s21_decimal_extended *b);
void s21_set_sign_extended(s21_decimal_extended *dec, int sign);
int s21_compare_mantissa_extended(const s21_decimal_extended *a,
                                  const s21_decimal_extended *b);

// div
void s21_binary_division_extended(s21_decimal_extended *dividend,
                                  s21_decimal_extended *divisor,
                                  s21_decimal_extended *quotient,
                                  s21_decimal_extended *remainder);
void s21_prepare_division(s21_decimal *v1, s21_decimal *v2, int *sign);
void s21_increase_precision(s21_decimal_extended *ext1,
                            const s21_decimal_extended *ext2, int *result_scale,
                            int *precision_added, int scale1, int scale2);
void s21_apply_bank_rounding(s21_decimal_extended *quotient,
                             const s21_decimal_extended *remainder,
                             const s21_decimal_extended *divisor);

// mul
void s21_remove_trailing_zeros_mul(uint32_t *result, int *scale);
int s21_try_reduce_scale(uint32_t *result, int *scale);
void s21_multiply_mantissas(const s21_decimal *value_1,
                            const s21_decimal *value_2, uint32_t *result);
int s21_check_mul_args(const s21_decimal *res, int scale_sum);

// from float to decimal
int s21_is_invalid_float(float src);
void s21_normalize_float(double *value, int *scale);
void s21_round_double_to_significant(double *value, int *scale);
void s21_double_to_decimal_extended(double value, s21_decimal_extended *ext);
void s21_remove_trailing_zeros_fdecimal(s21_decimal_extended *ext, int *scale);

// from decimal to float
int s21_from_decimal_to_long_double(s21_decimal src, long double *dst);

#endif  // s21_decimal_extended_H
