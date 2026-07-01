#include "s21_help.h"

// SIMPLES

s21_decimal s21_make_decimal(int lo, int mid, int hi, int sign, int scale) {
  s21_decimal d = {0};
  d.bits[0] = lo;
  d.bits[1] = mid;
  d.bits[2] = hi;
  s21_set_sign(&d, sign);
  s21_set_scale(&d, scale);
  return d;
}

void s21_clear_bits(s21_decimal *value) {
  for (int i = 0; i < 4; i++) {
    value->bits[i] = 0;
  }
}

int s21_is_zero(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

int s21_get_bit(s21_decimal value, int position) {
  if (s21_is_valid_position(position) == 0) {
    return 1;
  }
  int word = position / 32;
  int bit = position % 32;
  return (value.bits[word] >> bit) & 1;
}

int s21_set_bit(s21_decimal *value, int position, int bit) {
  if (s21_is_valid_position(position) == 0) {
    return 1;
  }
  int word = position / 32;
  int shift = position % 32;
  if (bit) {
    value->bits[word] |= (1u << shift);
  } else {
    value->bits[word] &= ~(1u << shift);
  }
  return 0;
}

int s21_get_sign(s21_decimal value) { return s21_get_bit(value, 127); }

int s21_set_sign(s21_decimal *value, int sign) {
  s21_set_bit(value, 127, sign ? 1 : 0);
  return 0;
}

int s21_get_scale(s21_decimal value) { return (value.bits[3] >> 16) & 0xFF; }

int s21_set_scale(s21_decimal *value, int scale) {
  if (s21_is_valid_scale(scale) == 0) {
    return 1;
  }

  value->bits[3] &= ~(0xFF << 16);  // Сброс предыдущего значения scale
  value->bits[3] |= (scale << 16);  // Установка нового значения scale
  return 0;
}

int s21_is_valid_position(int position) {
  return (position >= 0 && position < 128);
}

int s21_is_valid_scale(int scale) { return (scale >= 0 && scale <= 28); }

int s21_is_valid_decimal(s21_decimal src) {
  return !(src.bits[3] & VALIDATION_MASK) &&
         (s21_is_valid_scale(s21_get_scale(src)));
}

int s21_is_valid_int(double src) { return (src >= INT_MIN && src <= INT_MAX); }

// COMPARISON
int s21_compare_mantissa(s21_decimal value_1, s21_decimal value_2) {
  for (int i = 2; i >= 0; i--) {
    unsigned int part1 = value_1.bits[i];
    unsigned int part2 = value_2.bits[i];
    if (part1 > part2) return 1;   // Мантисса value_1 > value_2
    if (part1 < part2) return -1;  // Мантисса value_1 < value_2
  }
  return 0;  // Мантиссы равны
}

int s21_align_scales(s21_decimal *value_1, s21_decimal *value_2) {
  int scale_a = s21_get_scale(*value_1);
  int scale_b = s21_get_scale(*value_2);

  while (scale_a < scale_b) {
    if (s21_can_multiply_by_10(*value_1)) {
      s21_multiply_by_10(value_1);
      scale_a++;
    } else if (s21_div_10_t(value_2)) {
      scale_b--;
    } else {
      break;
    }
  }

  while (scale_b < scale_a) {
    if (s21_can_multiply_by_10(*value_2)) {
      s21_multiply_by_10(value_2);
      scale_b++;
    } else if (s21_div_10_t(value_1)) {
      scale_a--;
    } else {
      break;
    }
  }

  s21_set_scale(value_1, scale_a);
  s21_set_scale(value_2, scale_b);

  return scale_a != scale_b;  // 0 — успех, 1 — не удалось выровнять
}

void s21_multiply_by_10(s21_decimal *value) {
  const unsigned long long temp[3] = {(unsigned long long)value->bits[0],
                                      (unsigned long long)value->bits[1],
                                      (unsigned long long)value->bits[2]};
  unsigned long long carry = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long long product = temp[i] * 10ULL + carry;
    value->bits[i] = (unsigned int)(product & 0xFFFFFFFF);
    carry = product >> 32;
  }
}

int s21_can_multiply_by_10(s21_decimal value) {
  const unsigned long long temp[3] = {(unsigned long long)value.bits[0],
                                      (unsigned long long)value.bits[1],
                                      (unsigned long long)value.bits[2]};
  unsigned long long carry = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long long product = temp[i] * 10ULL + carry;
    carry = product >> 32;  // Перенос в старший разряд
    if (carry && i == 2) return 0;  // Переполнение
  }
  return 1;
}

int s21_div_10_t(s21_decimal *value) {
  if (!value) return 0;

  uint64_t remainder = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t cur = ((uint64_t)remainder << 32) | value->bits[i];
    value->bits[i] = (uint32_t)(cur / 10);
    remainder = cur % 10;
  }

  return 1;
}

// TRUNCATE
void s21_div_10(s21_decimal *value) {
  uint64_t remainder = 0;
  for (int i = 2; i >= 0; i--) {
    uint64_t current = (remainder << 32) | value->bits[i];
    value->bits[i] = (uint32_t)(current / 10);
    remainder = current % 10;
  }

  // уменьшить scale на 1
  int scale = s21_get_scale(*value);
  if (scale > 0) {
    scale--;
    s21_set_scale(value, scale);  // установить новый scale
  }
}

// ADD
void s21_decimal_to_extended(s21_decimal *src, s21_decimal_extended *dst) {
  for (int i = 0; i < 3; i++) dst->bits[i] = src->bits[i];
  for (int i = 3; i < 6; i++) dst->bits[i] = 0;
  dst->bits[6] = src->bits[3];
}

void s21_remove_trailing_zeros_extended(s21_decimal_extended *val) {
  int scale = (val->bits[6] >> 16) & 0xFF;
  while (scale > 0) {
    s21_decimal_extended tmp = *val;
    uint64_t rem = 0;
    for (int i = 5; i >= 0; i--) {
      uint64_t temp = (rem << 32) | tmp.bits[i];
      tmp.bits[i] = (uint32_t)(temp / 10);
      rem = temp % 10;
    }
    if (rem != 0) break;
    *val = tmp;
    scale--;
  }
  val->bits[6] = (val->bits[6] & 0xFF00FFFF) | (scale << 16);
}

int s21_extended_to_decimal(s21_decimal_extended *src, s21_decimal *dst) {
  while (1) {
    s21_remove_trailing_zeros_extended(src);
    if (!(src->bits[3] || src->bits[4] || src->bits[5])) break;
    int scale = (src->bits[6] >> 16) & 0xFF;
    if (scale == 0) return 1;
    s21_decimal_extended tmp = *src;
    uint64_t rem = 0;
    for (int i = 5; i >= 0; i--) {
      uint64_t temp = (rem << 32) | tmp.bits[i];
      tmp.bits[i] = (uint32_t)(temp / 10);
      rem = temp % 10;
    }
    int need_round = 0;
    if (rem > 5 || (rem == 5 && (tmp.bits[0] & 1))) {
      need_round = 1;
    }
    if (need_round) {
      uint64_t carry = 1;
      for (int i = 0; i < 6 && carry; i++) {
        uint64_t sum = (uint64_t)tmp.bits[i] + carry;
        tmp.bits[i] = (uint32_t)(sum & 0xFFFFFFFF);
        carry = sum >> 32;
      }
    }
    tmp.bits[6] = (tmp.bits[6] & 0xFF00FFFF) | ((scale - 1) << 16);
    *src = tmp;
  }
  for (int i = 0; i < 3; i++) dst->bits[i] = src->bits[i];
  dst->bits[3] = src->bits[6];
  return 0;
}

void s21_add_extended(s21_decimal_extended *a, s21_decimal_extended *b,
                      s21_decimal_extended *res) {
  uint64_t carry = 0;
  for (int i = 0; i < 6; i++) {
    uint64_t sum = (uint64_t)a->bits[i] + b->bits[i] + carry;
    res->bits[i] = (uint32_t)(sum & 0xFFFFFFFFu);
    carry = sum >> 32;
  }
  res->bits[6] = a->bits[6];
}

void s21_subtract_extended(s21_decimal_extended *a, s21_decimal_extended *b,
                           s21_decimal_extended *res) {
  uint64_t borrow = 0;
  for (int i = 0; i < 6; i++) {
    uint64_t minuend = (uint64_t)a->bits[i];
    uint64_t subtrahend = (uint64_t)b->bits[i] + borrow;
    if (minuend < subtrahend) {
      res->bits[i] = (uint32_t)(0x100000000 + minuend - subtrahend);
      borrow = 1;
    } else {
      res->bits[i] = (uint32_t)(minuend - subtrahend);
      borrow = 0;
    }
  }
  res->bits[6] = a->bits[6];
}

void s21_multiply_by_10_extended(s21_decimal_extended *val) {
  uint64_t carry = 0;
  for (int i = 0; i < 6; i++) {
    uint64_t product = (uint64_t)val->bits[i] * 10 + carry;
    val->bits[i] = (uint32_t)(product & 0xFFFFFFFF);
    carry = product >> 32;
  }
}

int s21_can_multiply_by_10_extended(s21_decimal_extended val) {
  uint64_t carry = 0;
  for (int i = 0; i < 6; i++) {
    uint64_t product = (uint64_t)val.bits[i] * 10 + carry;
    carry = product >> 32;
  }
  return (carry == 0);
}

void s21_align_scales_extended(s21_decimal_extended *a,
                               s21_decimal_extended *b) {
  int scale_a = (a->bits[6] >> 16) & 0xFF;
  int scale_b = (b->bits[6] >> 16) & 0xFF;
  while (scale_a < scale_b) {
    if (!s21_can_multiply_by_10_extended(*a)) break;
    s21_multiply_by_10_extended(a);
    scale_a++;
  }

  while (scale_b < scale_a) {
    if (!s21_can_multiply_by_10_extended(*b)) break;
    s21_multiply_by_10_extended(b);
    scale_b++;
  }
  a->bits[6] = (a->bits[6] & 0xFF00FFFF) | (scale_a << 16);
  b->bits[6] = (b->bits[6] & 0xFF00FFFF) | (scale_b << 16);
}

void s21_set_sign_extended(s21_decimal_extended *dec, int sign) {
  if (sign)
    dec->bits[6] |= (1U << 31);
  else
    dec->bits[6] &= ~(1U << 31);
}

// Сравнение мантисс 1 - greater; -1 - less; 0 - equal
int s21_compare_mantissa_extended(const s21_decimal_extended *a,
                                  const s21_decimal_extended *b) {
  for (int i = 5; i >= 0; i--) {
    if (a->bits[i] > b->bits[i]) return 1;
    if (a->bits[i] < b->bits[i]) return -1;
  }
  return 0;
}

// DIV
void s21_binary_division_extended(s21_decimal_extended *dividend,
                                  s21_decimal_extended *divisor,
                                  s21_decimal_extended *quotient,
                                  s21_decimal_extended *remainder) {
  memset(quotient, 0, sizeof(*quotient));
  memset(remainder, 0, sizeof(*remainder));

  // Копируем dividend в temp, будем сдвигать по битам

  for (int i = 191; i >= 0; i--) {
    // Сдвигаем остаток влево на 1 бит
    uint32_t carry = 0;
    for (int j = 5; j >= 0; j--) {
      uint64_t val = ((uint64_t)remainder->bits[j] << 1) | carry;
      carry = (remainder->bits[j] >> 31) & 1;
      remainder->bits[j] = (uint32_t)(val & 0xFFFFFFFF);
    }

    // Получаем текущий бит из делимого
    int word = i / 32;
    int bit = i % 32;
    int bit_value = (dividend->bits[word] >> bit) & 1;

    // Добавляем его в младший бит остатка
    remainder->bits[0] |= bit_value;

    // Если остаток >= делителя, вычитаем делитель и устанавливаем бит частного
    if (s21_compare_mantissa_extended(remainder, divisor) >= 0) {
      s21_decimal_extended temp_rem = *remainder;
      s21_subtract_extended(&temp_rem, divisor, remainder);

      // Устанавливаем бит в частном
      quotient->bits[word] |= (1u << bit);
    }
  }

  quotient->bits[6] = dividend->bits[6];  // передаём scale/sign
}

void s21_prepare_division(s21_decimal *v1, s21_decimal *v2, int *sign) {
  *sign = s21_get_sign(*v1) ^ s21_get_sign(*v2);
  s21_set_sign(v1, 0);
  s21_set_sign(v2, 0);
}

void s21_apply_bank_rounding(s21_decimal_extended *quotient,
                             const s21_decimal_extended *remainder,
                             const s21_decimal_extended *divisor) {
  s21_decimal_extended half = *divisor;
  for (int i = 0; i < 6; i++) {
    half.bits[i] >>= 1;
    if (i < 5 && (divisor->bits[i] & 1)) {
      half.bits[i + 1] |= 0x80000000;
    }
  }
  int val = s21_compare_mantissa_extended(remainder, &half);
  if (val > 0 || (val == 0 && (quotient->bits[0] & 1))) {
    s21_decimal_extended one = {0};
    one.bits[0] = 1;
    s21_decimal_extended tmp = {0};
    s21_add_extended(quotient, &one, &tmp);
    *quotient = tmp;
  }
}

void s21_increase_precision(s21_decimal_extended *ext1,
                            const s21_decimal_extended *ext2, int *result_scale,
                            int *precision_added, int scale1, int scale2) {
  const int MIN_PRECISION = 6;
  *precision_added = 0;
  *result_scale = scale1 - scale2;

  while ((*precision_added < MIN_PRECISION ||
          s21_compare_mantissa_extended(ext1, ext2) < 0) &&
         (scale1 + *precision_added < 28)) {
    if (!s21_can_multiply_by_10_extended(*ext1)) break;
    s21_multiply_by_10_extended(ext1);
    (*precision_added)++;
    (*result_scale)++;
  }
}

/// MUL
int s21_check_mul_args(const s21_decimal *res, int scale_sum) {
  if (!res || scale_sum > 28) return 1;
  return 0;
}

void s21_multiply_mantissas(const s21_decimal *value_1,
                            const s21_decimal *value_2, uint32_t *result) {
  for (int i = 0; i < 96; i++) {
    if (s21_get_bit(*value_2, i)) {
      uint32_t buffer[6] = {0};
      for (int j = 0; j < 96; j++) {
        if (s21_get_bit(*value_1, j)) {
          int pos = i + j;
          if (pos >= 192) continue;
          int index = pos / 32;
          int offset = pos % 32;

          uint64_t bit = (uint64_t)1 << offset;
          uint64_t sum = (uint64_t)buffer[index] + bit;
          buffer[index] = (uint32_t)(sum & 0xFFFFFFFF);

          for (int k = index + 1; k < 6 && (sum >> 32); k++) {
            sum = (uint64_t)buffer[k] + 1;
            buffer[k] = (uint32_t)(sum & 0xFFFFFFFF);
          }
        }
      }

      uint64_t carry = 0;
      for (int k = 0; k < 6; k++) {
        uint64_t temp = (uint64_t)result[k] + buffer[k] + carry;
        result[k] = (uint32_t)(temp & 0xFFFFFFFF);
        carry = temp >> 32;
      }
    }
  }
}

int s21_try_reduce_scale(uint32_t *result, int *scale) {
  while (*scale > 0 && (result[3] || result[4] || result[5])) {
    uint64_t rem = 0;
    for (int i = 5; i >= 0; i--) {
      uint64_t temp = (rem << 32) | result[i];
      result[i] = (uint32_t)(temp / 10);
      rem = temp % 10;
    }
    if (rem >= 5) {
      uint64_t carry = 1;
      for (int i = 0; i < 6 && carry; i++) {
        uint64_t sum = (uint64_t)result[i] + carry;
        result[i] = (uint32_t)(sum & 0xFFFFFFFF);
        carry = sum >> 32;
      }
    }
    (*scale)--;
  }
  return (result[3] || result[4] || result[5]);
}

void s21_remove_trailing_zeros_mul(uint32_t *result, int *scale) {
  while (*scale > 0 && result[0] % 10 == 0 && result[1] == 0 &&
         result[2] == 0) {
    result[0] /= 10;
    (*scale)--;
  }
}

// FROM_FLOAT_TO_DECIMAL
int s21_is_invalid_float(float src) {
  return (!isfinite(src) || (fabsf(src) < MIN_DECIMAL_FLOAT && src != 0.0f) ||
          fabsf(src) > MAX_DECIMAL_FLOAT);
}

void s21_normalize_float(double *value, int *scale) {
  while (*scale < 28 && floor(*value) != *value) {
    *value *= 10.0;
    (*scale)++;
  }
}

void s21_round_double_to_significant(double *value, int *scale) {
  int digits = (int)log10(*value) + 1;
  if (digits > 7) {
    int diff = digits - 7;
    double pow10 = pow(10.0, diff);
    *value = round(*value / pow10);
    if (*scale >= diff)
      *scale -= diff;
    else {
      *value = *value * pow10;
      *scale = 0;
    }
  }
}

void s21_double_to_decimal_extended(double value, s21_decimal_extended *ext) {
  for (int i = 0; i < 3; i++) {
    ext->bits[i] = (uint32_t)fmod(value, 4294967296.0);  // 2^32
    value = floor(value / 4294967296.0);
  }
}

void s21_remove_trailing_zeros_fdecimal(s21_decimal_extended *ext, int *scale) {
  while (*scale > 0 && ext->bits[1] == 0 && ext->bits[2] == 0 &&
         ext->bits[0] % 10 == 0) {
    ext->bits[0] /= 10;
    (*scale)--;
  }
}
// FROM_DECIMAL_TO_FLOAT
int s21_from_decimal_to_long_double(s21_decimal src, long double *dst) {
  int err = 1;
  int exponent = s21_get_scale(src);
  if (dst && exponent <= 28 && s21_is_valid_decimal(src)) {
    long double pow2_32 = powl(2.0L, 32);  // 2^32 = 4 294 967 296.0
    long double pow2_64 =
        powl(2.0L, 64);  // 2^64 = 18 446 744 073 709 551 616.0
    long double hi =
        (long double)(unsigned int)src.bits[2] * pow2_64;  // Старшие биты
    long double mid =
        (long double)(unsigned int)src.bits[1] * pow2_32;  // Средние биты
    long double lo = (long double)(unsigned int)src.bits[0];  // Младшие биты

    // Складываем от старшего к младшему
    long double result = hi;
    if (mid > 0) result += mid;  // Добавляем только если не нуль
    if (lo > 0) result += lo;
    if (exponent > 0) {
      const long double powers_of_10[] = {
          1e0L,  1e1L,  1e2L,  1e3L,  1e4L,  1e5L,  1e6L,  1e7L,  1e8L,  1e9L,
          1e10L, 1e11L, 1e12L, 1e13L, 1e14L, 1e15L, 1e16L, 1e17L, 1e18L, 1e19L,
          1e20L, 1e21L, 1e22L, 1e23L, 1e24L, 1e25L, 1e26L, 1e27L, 1e28L};
      result /= powers_of_10[exponent];
    }
    if (s21_get_sign(src)) {
      result *= -1;
    }
    *dst = result;
    err = 0;
  }
  return err;
}
