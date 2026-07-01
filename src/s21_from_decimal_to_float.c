#include "s21_help.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int err = 1;
  long double result = 0.0;

  if (dst && !s21_from_decimal_to_long_double(src, &result)) {
    if (fabsl(result) < MIN_DECIMAL_FLOAT && result != 0.0) {
      // Обработка денормализованных чисел
      *dst = (float)(roundl(result * 1e7) / 1e7);
      err = 0;
    } else if (fabsl(result) > MAX_DECIMAL_FLOAT || !isfinite(result)) {
      // Переполнение или NaN
      err = 1;
    } else {
      // Нормальные значения
      *dst = (float)(roundl(result * 1e7) / 1e7);
      err = 0;
    }
  }

  return err;
}
