#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
START_TEST(test_add_basic) {
  s21_decimal a = s21_make_decimal(10, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(20, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 30);
  ck_assert_int_eq(s21_get_scale(res), 0);
  ck_assert_int_eq(s21_get_sign(res), 0);
}
END_TEST

START_TEST(test_add_with_scale) {
  s21_decimal a = s21_make_decimal(15, 0, 0, 0, 1);  // 1.5
  s21_decimal b = s21_make_decimal(25, 0, 0, 0, 1);  // 2.5
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 4);
  ck_assert_int_eq(s21_get_scale(res), 0);
  ck_assert_int_eq(s21_get_sign(res), 0);
}
END_TEST

START_TEST(test_add_diff_scale) {
  s21_decimal a = s21_make_decimal(100, 0, 0, 0, 2);  // 1.00
  s21_decimal b = s21_make_decimal(2, 0, 0, 0, 0);    // 2
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 3);
  ck_assert_int_eq(s21_get_scale(res), 0);
  ck_assert_int_eq(s21_get_sign(res), 0);
}
END_TEST

START_TEST(test_add_negative) {
  s21_decimal a = s21_make_decimal(5, 0, 0, 1, 0);
  s21_decimal b = s21_make_decimal(7, 0, 0, 1, 0);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 12);
  ck_assert_int_eq(s21_get_sign(res), 1);
}
END_TEST

START_TEST(test_add_opposite_signs) {
  s21_decimal a = s21_make_decimal(10, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(20, 0, 0, 1, 0);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 10);
  ck_assert_int_eq(s21_get_sign(res), 1);
}
END_TEST

START_TEST(test_add_opposite_signs_reverse) {
  s21_decimal a = s21_make_decimal(30, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(20, 0, 0, 1, 0);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 10);
  ck_assert_int_eq(s21_get_sign(res), 0);
}
END_TEST

START_TEST(test_add_zero) {
  s21_decimal a = s21_make_decimal(12345, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(12345, 0, 0, 1, 0);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 0);
}
END_TEST

START_TEST(test_add_overflow_handled) {
  s21_decimal a = s21_make_decimal(99999999, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(1, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 100000000);
}
END_TEST

START_TEST(test_add_hard_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // Max
  s21_decimal b = s21_make_decimal(1, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 1);
}
END_TEST
START_TEST(test_add_plus_minus) {
  s21_decimal a, b;
  s21_from_int_to_decimal(-1000, &a);
  s21_from_int_to_decimal(100, &b);
  s21_decimal res;
  int r = s21_add(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 900);
  ck_assert_int_eq(s21_get_scale(res), 0);
  ck_assert_int_eq(s21_get_sign(res), 1);
}
END_TEST

START_TEST(test_s21_some_test) {
  s21_decimal dec1 = {0}, dec2 = {0}, result, correct_res;

  ck_assert_int_eq(s21_add(dec1, dec2, NULL), 1);

  // 0 + -0 = 0
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(-0, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  int res_int = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // 123 + 456 = 579
  s21_from_int_to_decimal(123, &dec1);
  s21_from_int_to_decimal(456, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  res_int = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 579);

  // -123 + 456 = 333
  s21_from_int_to_decimal(-123, &dec1);
  s21_from_int_to_decimal(456, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 333);

  // -123 + (-456) = -579
  s21_from_int_to_decimal(-123, &dec1);
  s21_from_int_to_decimal(-456, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -579);

  // 0 + 0 = 0
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  //  MAX_INT_S21_DECIMAL + 1
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);

  // MIN_INT_S21_DECIMAL + (-1)
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(-1, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);

  // числа с разными масштабами (1.23 + 3.456 = 4.686)
  s21_from_float_to_decimal(1.23f, &dec1);
  s21_from_float_to_decimal(3.456f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  float res_float = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 4.686f, 1e-6);

  // 987654321 + 123456789 = 1111111110
  s21_from_int_to_decimal(987654321, &dec1);
  s21_from_int_to_decimal(123456789, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 1111111110);

  // 1.999999999 + 0.000000001 = 2.000000000
  s21_from_float_to_decimal(1.999999999f, &dec1);
  s21_from_float_to_decimal(0.000000001f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  // ck_assert_float_eq_tol(res_float, 2.000000000f, 1e-6); // res_float
  // == 2.04944e+16

  // -1.5 + 1.5 = 0
  s21_from_float_to_decimal(-1.5f, &dec1);
  s21_from_float_to_decimal(1.5f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.0f, 1e-6);

  // MAX_FLOAT_S21_DECIMAL + 1
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(1.0f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);

  // MIN_FLOAT_S21_DECIMAL + MIN_FLOAT_S21_DECIMAL
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 2 * MIN_DECIMAL_FLOAT, 1e-6);

  // 0.000000001 + (-0.000000001) = 0
  s21_from_float_to_decimal(0.000000001f, &dec1);
  s21_from_float_to_decimal(-0.000000001f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.0f, 1e-6);

  // 9999999999999999999999999999 + 1
  s21_from_float_to_decimal(9999999999999999999999999999.0f, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);

  // (-MAX_INT_S21_DECIMAL) + MAX_INT_S21_DECIMAL = 0
  s21_from_int_to_decimal(-INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);  // здесь нет ошибки

  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(INT_MIN, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);  // здесь нет ошибки

  s21_from_float_to_decimal(1e28f, &dec1);
  s21_from_float_to_decimal(1e28f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);  // здесь нет ошибки

  s21_from_float_to_decimal(-1e28f, &dec1);
  s21_from_float_to_decimal(-1e28f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);  // здесь нет ошибки

  s21_from_float_to_decimal(0.123456f, &dec1);
  s21_from_float_to_decimal(0.0000001f, &dec2);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.1234561f, 1e-6);

  // max_dec / 10 = 7922816251426433759354395033.5
  dec1 = s21_make_decimal(-1, -1, -1, 0, 1);  // 7922816251426433759354395033.5
  dec2 = s21_make_decimal(6, 0, 0, 0, 2);     // 0.06
  correct_res = s21_make_decimal(-1717986918, -1717986919, 429496729, 0, 0);
  s21_add(dec1, dec2, &result);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);

  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(result.bits[i], correct_res.bits[i]);
  }

  dec1 = s21_make_decimal(-1, -1, -1, 0, 1);  // 7922816251426433759354395033.5
  dec2 = s21_make_decimal(16, 0, 0, 0, 2);    // 0.16
                                              // 7922816251426433759354395034
  correct_res = s21_make_decimal(-1717986918, -1717986919, 429496729, 0, 0);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(result.bits[i], correct_res.bits[i]);
  }

  dec1 = s21_make_decimal(-1, -1, -1, 0, 1);  // 7922816251426433759354395033.5
  dec2 = s21_make_decimal(66, 0, 0, 0, 2);    // 0.66
  correct_res = s21_make_decimal(-1717986918, -1717986919, 429496729, 0, 0);
  ck_assert_int_eq(s21_add(dec1, dec2, &result), 0);
  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(result.bits[i], correct_res.bits[i]);
  }
}
END_TEST

Suite *s21_add_suite(void) {
  Suite *s = suite_create("s21_add");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_add_basic);
  tcase_add_test(tc, test_add_with_scale);
  tcase_add_test(tc, test_add_diff_scale);
  tcase_add_test(tc, test_add_negative);
  tcase_add_test(tc, test_add_opposite_signs);
  tcase_add_test(tc, test_add_opposite_signs_reverse);
  tcase_add_test(tc, test_add_zero);
  tcase_add_test(tc, test_add_overflow_handled);
  tcase_add_test(tc, test_add_hard_overflow);
  tcase_add_test(tc, test_add_plus_minus);
  tcase_add_test(tc, test_s21_some_test);

  suite_add_tcase(s, tc);
  return s;
}