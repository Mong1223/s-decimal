#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
// float ≈ 123.45
START_TEST(test_simple_fraction) {
  s21_decimal d = s21_make_decimal(12345, 0, 0, 0, 2);
  float f = 0.0f;
  int r = s21_from_decimal_to_float(d, &f);
  ck_assert_int_eq(r, 0);
  ck_assert_float_eq_tol(f, 123.45f, 1e-5f);
}
END_TEST

// float ≈ -999.99
START_TEST(test_negative_fraction) {
  s21_decimal d = s21_make_decimal(99999, 0, 0, 1, 2);
  float f = 0.0f;
  int r = s21_from_decimal_to_float(d, &f);
  ck_assert_int_eq(r, 0);
  ck_assert_float_eq_tol(f, -999.99f, 1e-5f);
}
END_TEST

// float = 0
START_TEST(test_zero) {
  s21_decimal d = s21_make_decimal(0, 0, 0, 0, 0);
  float f = 123.456f;
  int r = s21_from_decimal_to_float(d, &f);
  ck_assert_int_eq(r, 0);
  ck_assert_float_eq(f, 0.0f);
}
END_TEST

// float = -0
START_TEST(test_negative_zero) {
  s21_decimal d = s21_make_decimal(0, 0, 0, 1, 0);
  float f = 123.456f;
  int r = s21_from_decimal_to_float(d, &f);
  ck_assert_int_eq(r, 0);
  ck_assert(isnan(f) == 0);
  ck_assert(signbit(f));
  ck_assert_float_eq(f, -0.0f);
}
END_TEST

// Большое число (макс в пределах float)
START_TEST(test_large_number) {
  s21_decimal d = s21_make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0);
  float f = 0.0f;
  int r = s21_from_decimal_to_float(d, &f);
  ck_assert_int_eq(r, 0);
  ck_assert_float_eq_tol(f, 1.8446744e+19f, 1e12f);  // возможна потеря точности
}
END_TEST

// Неверный decimal (scale = 255)
START_TEST(test_invalid_decimal_scale) {
  s21_decimal d = {0};
  d.bits[0] = 1;
  d.bits[3] = (255 << 16);

  float f = 0.0f;
  int r = s21_from_decimal_to_float(d, &f);
  ck_assert_int_eq(r, 1);
}
END_TEST

// NULL dst
START_TEST(test_null_pointer) {
  s21_decimal d = s21_make_decimal(123, 0, 0, 0, 0);
  int r = s21_from_decimal_to_float(d, NULL);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_s21_some_test) {
  s21_decimal dec = {0};
  float result;

  ck_assert_int_eq(s21_from_decimal_to_float(dec, NULL), 1);

  s21_from_int_to_decimal(-0, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 0.0f, 1e-6);

  s21_from_int_to_decimal(123, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 123.0f, 1e-6);

  s21_from_int_to_decimal(12345, &dec);
  s21_set_scale(&dec, 2);  // 123.45
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 123.45f, 1e-6);

  s21_from_int_to_decimal(-9876, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, -9876.0f, 1e-6);

  s21_from_int_to_decimal(1, &dec);
  s21_set_scale(&dec, 5);  // 0.00001
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 0.00001f, 1e-6);

  s21_from_int_to_decimal(987654321, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 987654321.0f, 1e-6);

  s21_from_int_to_decimal(INT_MAX, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, INT_MAX, 1e-6);

  s21_from_int_to_decimal(INT_MIN, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, INT_MIN, 1e-6);

  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 7.922816e28, 1e-6);

  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, MIN_DECIMAL_FLOAT, 1e-6);

  // (scale = 0)
  s21_from_int_to_decimal(123456, &dec);
  s21_set_scale(&dec, 0);
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 123456.0f, 1e-6);

  // (scale = 28)
  s21_from_int_to_decimal(1, &dec);
  s21_set_scale(&dec, 28);  // 1e-28
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, 1e-28f, 1e-6);

  s21_from_int_to_decimal(-98765, &dec);
  s21_set_scale(&dec, 3);  // -98.765
  ck_assert_int_eq(s21_from_decimal_to_float(dec, &result), 0);
  ck_assert_float_eq_tol(result, -98.765f, 1e-6);

  // проверка ошибки (некорректный decimal)
  s21_decimal invalid = {0};
  invalid.bits[3] = 0xFFFFFFFF;  // Некорректные биты
  ck_assert_int_eq(s21_from_decimal_to_float(invalid, &result), 1);
}
END_TEST

Suite *from_decimal_to_float_suite(void) {
  Suite *s = suite_create("from_decimal_to_float");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_simple_fraction);
  tcase_add_test(tc, test_negative_fraction);
  tcase_add_test(tc, test_zero);
  tcase_add_test(tc, test_negative_zero);
  tcase_add_test(tc, test_large_number);
  tcase_add_test(tc, test_invalid_decimal_scale);
  tcase_add_test(tc, test_null_pointer);
  tcase_add_test(tc, test_s21_some_test);

  suite_add_tcase(s, tc);
  return s;
}
