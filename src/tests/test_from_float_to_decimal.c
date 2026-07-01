#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"

int get_decimal_lo(const s21_decimal *d) { return d->bits[0]; }
int get_decimal_mid(const s21_decimal *d) { return d->bits[1]; }

START_TEST(test_simple_integer) {
  s21_decimal d = {0};
  float f = 100.0f;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(get_decimal_lo(&d), 100);
  ck_assert_int_eq(get_decimal_mid(&d), 0);
  ck_assert_int_eq(s21_get_scale(d), 0);
  ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST

START_TEST(test_simple_fraction) {
  s21_decimal d = {0};
  float f = 123.45f;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(get_decimal_lo(&d), 12345);
  ck_assert_int_eq(s21_get_scale(d), 2);
  ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST

START_TEST(test_negative_fraction) {
  s21_decimal d = {0};
  float f = -42.5f;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(get_decimal_lo(&d), 425);
  ck_assert_int_eq(s21_get_scale(d), 1);
  ck_assert_int_eq(s21_get_sign(d), 1);
}
END_TEST

START_TEST(test_zero) {
  s21_decimal d = {0};
  float f = 0.0f;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(get_decimal_lo(&d), 0);
  ck_assert_int_eq(s21_get_scale(d), 0);
}
END_TEST

START_TEST(test_negative_zero) {
  s21_decimal d = {0};
  float f = -0.0f;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(get_decimal_lo(&d), 0);
  ck_assert_int_eq(s21_get_sign(d), 1);
}
END_TEST

START_TEST(test_too_small) {
  s21_decimal d = {0};
  float f = 1e-29f;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_too_big) {
  s21_decimal d = {0};
  float f = 8e28f;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_nan) {
  s21_decimal d = {0};
  float f = NAN;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_inf) {
  s21_decimal d = {0};
  float f = INFINITY;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_neg_inf) {
  s21_decimal d = {0};
  float f = -INFINITY;
  int r = s21_from_float_to_decimal(f, &d);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_null_ptr) {
  float f = 1.23f;
  int r = s21_from_float_to_decimal(f, NULL);
  ck_assert_int_eq(r, 1);
}
END_TEST
// some tests
START_TEST(test_s21_some_test) {
  s21_decimal dec;

  ck_assert_int_eq(s21_from_float_to_decimal(NAN, &dec), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(INFINITY, &dec), 1);

  float num0 = 0;
  ck_assert_int_eq(s21_from_float_to_decimal(num0, NULL), 1);

  float num1 = 123.0f;
  ck_assert_int_eq(s21_from_float_to_decimal(num1, &dec), 0);

  int int_val = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(dec, &int_val), 0);
  ck_assert_int_eq(int_val, 123);
  ck_assert_int_eq(s21_get_scale(dec), 0);

  float num2 = 456.78f;
  ck_assert_int_eq(s21_from_float_to_decimal(num2, &dec), 0);

  float float_val = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &float_val), 0);
  ck_assert_float_eq_tol(float_val, num2, 1e-6);

  float num3 = -789.45f;
  ck_assert_int_eq(s21_from_float_to_decimal(num3, &dec), 0);
  ck_assert_int_eq(s21_get_sign(dec), 1);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &float_val), 0);
  ck_assert_float_eq_tol(float_val, num3, 1e-6);

  // -0
  float num4 = -0.0f;
  ck_assert_int_eq(s21_from_float_to_decimal(num4, &dec), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &float_val), 0);
  ck_assert_float_eq_tol(float_val, num4, 1e-6);
  ck_assert_int_eq(s21_get_scale(dec), 0);
  ck_assert_int_eq(s21_get_sign(dec), 1);

  float num5 = INT_MAX;
  ck_assert_int_eq(s21_from_float_to_decimal(num5, &dec), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(dec, &int_val), 1);

  float num6 = INT_MIN;
  ck_assert_int_eq(s21_from_float_to_decimal(num6, &dec), 0);

  ck_assert_int_eq(s21_from_decimal_to_int(dec, &int_val), 1);

  float num7 = MAX_DECIMAL_FLOAT;
  ck_assert_int_eq(s21_from_float_to_decimal(num7, &dec), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &float_val), 0);
  ck_assert_float_eq_tol(float_val, 7.922816e28, 1e+6);

  float num8 = MIN_DECIMAL_FLOAT;
  ck_assert_int_eq(s21_from_float_to_decimal(num8, &dec), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &float_val), 0);

  // число с большим масштабом
  float num9 = 0.00000123f;
  ck_assert_int_eq(s21_from_float_to_decimal(num9, &dec), 0);
  ck_assert_int_eq(s21_get_scale(dec), 8);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &float_val), 0);
  ck_assert_float_eq_tol(float_val, num9, 1e-6);

  // число, округляющееся в 0
  float num10 = 1e-30f;  // Слишком маленькое для decimal
  ck_assert_int_eq(s21_from_float_to_decimal(num10, &dec), 1);

  // проверка корректности экспоненты
  float num11 = 9876543.21f;
  ck_assert_int_eq(s21_from_float_to_decimal(num11, &dec), 0);

  ck_assert_int_eq(s21_from_decimal_to_float(dec, &float_val), 0);
  ck_assert_float_eq_tol(float_val, num11, 1e-6);
}
END_TEST
Suite *from_float_to_decimal_suite(void) {
  Suite *s = suite_create("from_float_to_decimal");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_simple_integer);
  tcase_add_test(tc, test_simple_fraction);
  tcase_add_test(tc, test_negative_fraction);
  tcase_add_test(tc, test_zero);
  tcase_add_test(tc, test_negative_zero);
  tcase_add_test(tc, test_too_small);
  tcase_add_test(tc, test_too_big);
  tcase_add_test(tc, test_nan);
  tcase_add_test(tc, test_inf);
  tcase_add_test(tc, test_neg_inf);
  tcase_add_test(tc, test_null_ptr);
  tcase_add_test(tc, test_s21_some_test);
  suite_add_tcase(s, tc);
  return s;
}
