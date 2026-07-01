#include <check.h>
#include <limits.h>
#include <math.h>

#include "../s21_decimal.h"
#include "../s21_help.h"

START_TEST(test_positive) {
  s21_decimal d = s21_make_decimal(123, 0, 0, 0, 0);
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(out, 123);
}
END_TEST

START_TEST(test_negative) {
  s21_decimal d = s21_make_decimal(321, 0, 0, 1, 0);
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(out, -321);
}
END_TEST

START_TEST(test_fraction_truncate) {
  s21_decimal d = s21_make_decimal(9999, 0, 0, 0, 0);
  s21_set_scale(&d, 2);  // 99.99 -> 99
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(out, 99);
}
END_TEST

START_TEST(test_zero) {
  s21_decimal d = s21_make_decimal(0, 0, 0, 0, 0);
  int out = -1;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(test_null_ptr) {
  s21_decimal d = s21_make_decimal(100, 0, 0, 0, 0);
  int result = s21_from_decimal_to_int(d, NULL);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_overflow) {
  s21_decimal d = s21_make_decimal(0xFFFFFFFF, 1, 0, 0, 0);
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_underflow) {
  s21_decimal d = s21_make_decimal(0xFFFFFFFF, 1, 0, 1, 0);
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_invalid_scale) {
  s21_decimal d = s21_make_decimal(123, 0, 0, 0, 0);
  d.bits[3] = (255 << 16);  // scale = 255 (некорректно)
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_int_max_value) {
  s21_decimal d = s21_make_decimal(INT_MAX, 0, 0, 0, 0);
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(out, INT_MAX);
}
END_TEST

START_TEST(test_int_min_value) {
  s21_decimal d =
      s21_make_decimal((unsigned)INT_MAX + 1, 0, 0, 1, 0);  // -INT_MIN
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(out, INT_MIN);
}
END_TEST

START_TEST(test_fraction_causes_overflow) {
  s21_decimal d = {0};
  d.bits[0] = 0xFFFFFFFF;
  d.bits[1] = 1;
  // s21_set_scale(&d, 1);
  s21_set_sign(&d, 0);
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_large_scale_high_bits) {
  s21_decimal d = {0};
  d.bits[0] = 0xFFFFFFFF;
  d.bits[1] = 0xFFFFFFFF;
  d.bits[2] = 1;
  s21_set_scale(&d, 2);
  s21_set_sign(&d, 0);
  int out = 0;
  int result = s21_from_decimal_to_int(d, &out);
  ck_assert_int_eq(result, 1);
}
END_TEST
// someother tests
START_TEST(test_s21_some_test) {
  s21_decimal src = {0};
  int dst;
  int err;

  err = s21_from_decimal_to_int(src, NULL);
  ck_assert_int_eq(err, 1);

  src.bits[0] = 0;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 1U << 31;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, 0);

  src.bits[0] = 12345;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, 12345);

  src.bits[0] = 12345;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 1U << 31;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, -12345);

  src.bits[0] = INT_MAX;
  src.bits[1] = INT_MAX;
  src.bits[2] = INT_MAX;
  src.bits[3] = 0;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 1);

  src.bits[0] = INT_MAX;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 1U << 31;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, -2147483647);

  src.bits[0] = INT_MAX;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, INT_MAX);

  // src.bits[0] = 2147483648; // abs(INT_MIN) -2147483648
  src.bits[0] = (int)2147483648U;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 1U << 31;

  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, INT_MIN);

  src.bits[0] = 12345;
  src.bits[1] = 0;
  src.bits[2] = 1;  // слишком большое число для int
  src.bits[3] = 0;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 1);

  src.bits[0] = 0;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, 0);

  src.bits[0] = 987654321;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 5U << 16;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, 9876);

  src.bits[0] = 9876543;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = (3U << 16) | (1U << 31);  // Scale = 3, знак "-"
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, -9876);

  src.bits[0] = 9876;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 5U << 16;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, 0);

  src.bits[0] = 12345;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 255U << 16;  // некорректный скейл
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 1);
  ck_assert_int_eq(dst, 0);

  // в данном случае игнорирует некорректный scale, и просто записывает в dst
  // число 12345
  src.bits[0] = 12345;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 256 << 16;
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 1);

  // Число помещается в int, ожидаем успешное преобразование
  src.bits[0] = 9876543;
  src.bits[1] = 1;
  src.bits[2] = 0;
  src.bits[3] = (3 << 16) | (1U << 31);  // Scale = 3, знак "-"
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst, -4304843);

  // Число не помещается в int, err = 1
  src.bits[0] = 9876543;
  src.bits[1] = 1;
  src.bits[2] = 0;
  src.bits[3] = (1U << 31);  // Scale = 0, знак "-"
  err = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(err, 1);
}
END_TEST
Suite *from_decimal_to_int_suite(void) {
  Suite *s = suite_create("from_decimal_to_int");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_positive);
  tcase_add_test(tc, test_negative);
  tcase_add_test(tc, test_fraction_truncate);
  tcase_add_test(tc, test_zero);
  tcase_add_test(tc, test_null_ptr);
  tcase_add_test(tc, test_overflow);
  tcase_add_test(tc, test_underflow);
  tcase_add_test(tc, test_invalid_scale);
  tcase_add_test(tc, test_int_max_value);
  tcase_add_test(tc, test_int_min_value);
  tcase_add_test(tc, test_fraction_causes_overflow);
  tcase_add_test(tc, test_large_scale_high_bits);
  tcase_add_test(tc, test_s21_some_test);

  suite_add_tcase(s, tc);
  return s;
}
