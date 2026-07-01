#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
START_TEST(test_mul_basic) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  int res;
  s21_from_decimal_to_int(result, &res);
  ck_assert_int_eq(res, 15);
}
END_TEST

START_TEST(test_mul_zero) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(0, &a);
  s21_from_int_to_decimal(99999, &b);
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  ck_assert_int_eq(s21_get_bit(result, 0), 0);
  ck_assert(s21_is_zero(result));
}
END_TEST

START_TEST(test_mul_negative) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(2, &b);
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  int res;
  s21_from_decimal_to_int(result, &res);
  ck_assert_int_eq(res, -10);
}
END_TEST

START_TEST(test_mul_negative_both) {
  s21_decimal a, b, result;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(-2, &b);
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  int res;
  s21_from_decimal_to_int(result, &res);
  ck_assert_int_eq(res, 10);
}
END_TEST

START_TEST(test_mul_scale) {
  s21_decimal a = {{1234, 0, 0, 1 << 16}};  // 123.4
  s21_decimal b = {{10, 0, 0, 1 << 16}};    // 1.0
  s21_decimal result;
  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  ck_assert_int_eq(result.bits[0], 1234);
  ck_assert_int_eq(s21_get_scale(result), 1);
}
END_TEST

START_TEST(test_mul_overflow) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal result;
  int r = s21_mul(a, b, &result);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_mul_rounding_scale) {
  s21_decimal a = {{99999999, 0, 0, 0}};
  s21_decimal b = s21_make_decimal(10, 0, 0, 0, 1);  // scale 1
  s21_decimal result;
  int r = s21_mul(a, b, &result);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(result.bits[0], 99999999);
}
END_TEST

START_TEST(test_s21_some_test) {
  s21_decimal dec1 = {0}, dec2 = {0}, result;

  ck_assert_int_eq(s21_mul(dec1, dec2, NULL), 1);

  int res_int = 0;
  float res_float = 0.0f;

  // 12 * 12 = 144
  s21_from_int_to_decimal(12, &dec1);
  s21_from_int_to_decimal(12, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 144);

  // -5 * 3 = -15
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(3, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -15);

  // -7 * -8 = 56
  s21_from_int_to_decimal(-7, &dec1);
  s21_from_int_to_decimal(-8, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 56);

  // 0 * 12345 = 0
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(12345, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // MAX_INT_S21_DECIMAL * 2 -> ошибка переполнения
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(2, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int),
                   1);  // переполнение

  // MIN_INT_S21_DECIMAL * -1 -> ошибка переполнения
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(-1, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int),
                   1);  // переполнение

  // 1.5 * 2.5 = 3.75
  s21_from_float_to_decimal(1.5f, &dec1);
  s21_from_float_to_decimal(2.5f, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 3.75f, 1e-6);

  // 999999 * 999999 = 999998000001
  s21_from_int_to_decimal(999999, &dec1);
  s21_from_int_to_decimal(999999, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int),
                   1);  // переполнение
  ck_assert_int_eq(res_int, 0);

  // 0.1 * 0.2 = 0.02
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.02f, 1e-6);

  // -3.14 * 2 = -6.28
  s21_from_float_to_decimal(-3.14f, &dec1);
  s21_from_int_to_decimal(2, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, -6.28f, 1e-6);

  // 0.000000001 * 1000000000 = 1
  dec1 = s21_make_decimal(1, 0, 0, 0, 9);
  s21_from_int_to_decimal(1000000000, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 1);

  // 999999999999999999 * 2 -> ошибка переполнения
  s21_from_float_to_decimal(999999999999999999.0f, &dec1);
  s21_from_int_to_decimal(2, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int),
                   1);  // переполнение

  // -1 * -1 = 1
  s21_from_int_to_decimal(-1, &dec1);
  s21_from_int_to_decimal(-1, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 1);

  // 0.0001 * 0.0001 = 0.00000001
  s21_from_float_to_decimal(0.0001f, &dec1);
  s21_from_float_to_decimal(0.0001f, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.00000001f, 1e-6);

  // MAX_FLOAT_S21_DECIMAL * 0.1 -> не должно быть переполнения
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, MAX_DECIMAL_FLOAT * 0.1f, 1e+21);

  // Неверный указатель
  dec1 = (s21_decimal){{1, 2, 3, 4}};
  dec2 = (s21_decimal){{5, 6, 7, 8}};
  ck_assert_int_eq(s21_mul(dec1, dec2, NULL), 1);

  dec1 = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  dec2 = (s21_decimal){{2, 0, 0, 0}};
  s21_clear_bits(&result);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 1);  // 1 — overflow

  dec1 = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0x001C0000}};
  dec2 = (s21_decimal){{10, 0, 0, 0}};
  s21_clear_bits(&result);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);  // 1 — no overflow

  dec1 = (s21_decimal){{UINT_MAX, UINT_MAX, UINT_MAX, 0x80000000}};
  dec2 = (s21_decimal){{2, 0, 0, 0}};
  s21_clear_bits(&result);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 2);  // 2 — overflow

  // d1 = 1e18, d2 = 1e12 → 1e30 (слишком большое значение)
  s21_from_float_to_decimal(1e18, &dec1);
  s21_from_float_to_decimal(1e12, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 1);

  // d1 = 1e-14, d2 = 1e-14 → 1e-28 — граничный случай
  s21_from_float_to_decimal(1e-14, &dec1);
  s21_from_float_to_decimal(1e-14, &dec2);
  ck_assert_int_eq(s21_mul(dec1, dec2, &result), 0);
}
END_TEST

Suite *s21_mul_suite(void) {
  Suite *s = suite_create("s21_mul");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_mul_basic);
  tcase_add_test(tc, test_mul_zero);
  tcase_add_test(tc, test_mul_negative);
  tcase_add_test(tc, test_mul_negative_both);
  tcase_add_test(tc, test_mul_scale);
  tcase_add_test(tc, test_mul_overflow);
  tcase_add_test(tc, test_mul_rounding_scale);
  tcase_add_test(tc, test_s21_some_test);

  suite_add_tcase(s, tc);
  return s;
}
