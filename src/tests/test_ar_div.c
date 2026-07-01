#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
START_TEST(test_div_basic) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(10, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  int res;
  s21_from_decimal_to_int(result, &res);
  ck_assert_int_eq(res, 5);
}
END_TEST

START_TEST(test_div_fraction) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(4, &b);
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  float f;
  s21_from_decimal_to_float(result, &f);
  ck_assert_float_eq_tol(f, 0.25f, 1e-6);
}
END_TEST

START_TEST(test_div_negative) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(-9, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  int res;
  s21_from_decimal_to_int(result, &res);
  ck_assert_int_eq(res, -3);
}
END_TEST

START_TEST(test_div_zero_numerator) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(99, &b);
  ck_assert_int_eq(s21_div(a, b, &result), 0);
  ck_assert(s21_is_zero(result));
}
END_TEST

START_TEST(test_div_by_zero) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(1, &a);
  s21_from_int_to_decimal(0, &b);
  ck_assert_int_eq(s21_div(a, b, &result), 3);
}
END_TEST

START_TEST(test_s21_some_test) {
  s21_decimal dec1 = {0}, dec2 = {0}, result;

  ck_assert_int_eq(s21_div(dec1, dec2, NULL), 1);

  int res_int = 0;
  float res_float = 0.0f;

  s21_from_int_to_decimal(-100, &dec1);
  s21_from_int_to_decimal(25, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -4);

  s21_from_int_to_decimal(-72, &dec1);
  s21_from_int_to_decimal(-9, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 8);

  s21_from_int_to_decimal(1000000000, &dec1);
  s21_from_int_to_decimal(10, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 100000000);

  s21_from_int_to_decimal(1, &dec1);
  s21_from_int_to_decimal(10, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  s21_from_int_to_decimal(999, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 999);

  s21_from_int_to_decimal(-15, &dec1);
  s21_from_int_to_decimal(3, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -5);

  // 144 / 12 = 12
  s21_from_int_to_decimal(144, &dec1);
  s21_from_int_to_decimal(12, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 12);

  // -15 / 3 = -5
  s21_from_int_to_decimal(-15, &dec1);
  s21_from_int_to_decimal(3, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -5);

  // -56 / -8 = 7
  s21_from_int_to_decimal(-56, &dec1);
  s21_from_int_to_decimal(-8, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 7);

  // 0 / 12345 = 0
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(12345, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // Деление на 0 -> ошибка
  s21_from_int_to_decimal(123, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 3);

  // 5 / 2 = 2.5
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(2, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 2.5f, 1e-6);

  // 5.5 / 2 = 2.75
  s21_from_float_to_decimal(5.5f, &dec1);
  s21_from_int_to_decimal(2, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 2.75f, 1e-6);

  // 100 / 3 ≈ 33.333333
  s21_from_int_to_decimal(100, &dec1);
  s21_from_int_to_decimal(3, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 33.333333f, 1e-6);

  // 0.1 / 0.2 = 0.5
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.5f, 1e-6);

  // -3.14 / 2 = -1.57
  s21_from_float_to_decimal(-3.14f, &dec1);
  s21_from_int_to_decimal(2, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, -1.57f, 1e-6);

  // 1 / 3 ≈ 0.333333
  s21_from_int_to_decimal(1, &dec1);
  s21_from_int_to_decimal(3, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.333333f, 1e-6);

  // MAX_FLOAT_S21_DECIMAL / 10
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_int_to_decimal(10, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, MAX_DECIMAL_FLOAT / 10.0f, 1e+21);

  // 1e-7 / 1e-6 = 0.1
  s21_from_float_to_decimal(0.0000001f, &dec1);
  s21_from_float_to_decimal(0.000001f, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.1f, 1e-6);

  // 1000000000 / 0.000000001 = 1e18
  s21_from_int_to_decimal(1000000000, &dec1);
  s21_from_float_to_decimal(0.000000001f, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 1e18f, 1e12);

  // 1 / MAX_FLOAT_S21_DECIMAL -> малое число
  s21_from_int_to_decimal(1, &dec1);
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 1.0f / MAX_DECIMAL_FLOAT, 1e-12);

  // MIN_FLOAT_S21_DECIMAL / 10
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_int_to_decimal(10, &dec2);
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, MIN_DECIMAL_FLOAT / 10.0f, 1e-12);

  // Неверный указатель

  dec1 = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  dec2 = (s21_decimal){{2, 0, 0, 0}};
  result = (s21_decimal){0};

  int status = s21_div(dec1, dec2, &result);
  ck_assert_int_eq(status, 0);

  // результат: 39614081257132168796771975168
  res_float = 0.0f;
  s21_from_decimal_to_float(result, &res_float);
  ck_assert_float_eq_tol(res_float, 3.9614081257132168e+28f, 1e+22f);

  dec1 = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0x001C0000}};
  dec2 = (s21_decimal){{10, 0, 0, 0}};
  ck_assert_int_eq(s21_div(dec1, dec2, &result), 0);

  dec1 = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0x80000000}};
  dec2 = (s21_decimal){{1, 0, 0, 0}};
  ck_assert_int_eq(s21_div(dec2, dec1, &result), 0);
}
END_TEST

START_TEST(test_div_overflow_plus) {
  s21_decimal a, b, result;
  a = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  b = (s21_decimal){{5, 0, 0, 1 << 16}};
  ck_assert_int_eq(s21_div(a, b, &result), 1);
}
END_TEST

START_TEST(test_div_overflow_minus) {
  s21_decimal a, b, result;
  a = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0x80000000}};
  b = (s21_decimal){{5, 0, 0, 1 << 16}};
  ck_assert_int_eq(s21_div(a, b, &result), 2);
}
END_TEST

Suite *s21_div_suite(void) {
  Suite *s = suite_create("s21_div");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_div_basic);
  tcase_add_test(tc, test_div_fraction);
  tcase_add_test(tc, test_div_negative);
  tcase_add_test(tc, test_div_zero_numerator);
  tcase_add_test(tc, test_div_by_zero);
  tcase_add_test(tc, test_s21_some_test);
  tcase_add_test(tc, test_div_overflow_plus);
  tcase_add_test(tc, test_div_overflow_minus);

  suite_add_tcase(s, tc);
  return s;
}
