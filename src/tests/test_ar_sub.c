#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"

START_TEST(test_sub_basic) {
  // s21_decimal a = MAKE_DEC(30, 0, 0);
  // s21_decimal b = MAKE_DEC(10, 0, 0);
  s21_decimal a = s21_make_decimal(30, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(10, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 20);
  ck_assert_int_eq(s21_get_sign(res), 0);
}
END_TEST

START_TEST(test_sub_to_zero) {
  // s21_decimal a = MAKE_DEC(1000, 0, 0);
  // s21_decimal b = MAKE_DEC(1000, 0, 0);
  s21_decimal a = s21_make_decimal(1000, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(1000, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 0);
}
END_TEST

START_TEST(test_sub_negative_result) {
  // s21_decimal a = MAKE_DEC(15, 0, 0);
  // s21_decimal b = MAKE_DEC(20, 0, 0);
  s21_decimal a = s21_make_decimal(15, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(20, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 5);
  ck_assert_int_eq(s21_get_sign(res), 1);
}
END_TEST

START_TEST(test_sub_with_scale) {
  // s21_decimal a = MAKE_DEC(250, 1, 0);               // 25.0
  // s21_decimal b = MAKE_DEC(125, 1, 0);               // 12.5
  s21_decimal a = s21_make_decimal(250, 0, 0, 0, 1);  // 25.0
  s21_decimal b = s21_make_decimal(125, 0, 0, 0, 1);  // 12.5
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 125);
  ck_assert_int_eq(s21_get_scale(res), 1);
}
END_TEST

START_TEST(test_sub_diff_scale) {
  s21_decimal a = s21_make_decimal(300, 0, 0, 0, 2);  // 3.00
  s21_decimal b = s21_make_decimal(1, 0, 0, 0, 0);    // 1
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(s21_get_scale(res), 0);
}
END_TEST

START_TEST(test_sub_large_positive) {
  s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal b = s21_make_decimal(1, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 0xFFFFFFFE);
}
END_TEST

START_TEST(test_sub_negative_zero) {
  s21_decimal a = s21_make_decimal(0, 0, 0, 1, 0);
  s21_decimal b = s21_make_decimal(0, 0, 0, 0, 0);
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(s21_get_sign(res), 1);
}
END_TEST
START_TEST(test_sub_bank_round) {
  s21_decimal a = s21_make_decimal(4, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(5, 0, 0, 0, 1);
  s21_decimal res;
  int r = s21_sub(a, b, &res);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(res.bits[0], 35);
  ck_assert_int_eq(s21_get_sign(res), 0);
}
END_TEST

START_TEST(test_some_test) {
  s21_decimal dec1 = {0}, dec2 = {0}, result, correct_res;

  int res_int = 0;
  float res_float = 0.0f;

  ck_assert_int_eq(s21_sub(dec1, dec2, NULL), 1);

  // -0 - 0 = 0
  s21_from_int_to_decimal(-0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // 456 - 123 = 333
  s21_from_int_to_decimal(456, &dec1);
  s21_from_int_to_decimal(123, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 333);

  // 123 - 456 = -333
  s21_from_int_to_decimal(123, &dec1);
  s21_from_int_to_decimal(456, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -333);

  // -123 - (-456) = 333
  s21_from_int_to_decimal(-123, &dec1);
  s21_from_int_to_decimal(-456, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 333);

  // 0 - 0 = 0
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // MAX_INT_S21_DECIMAL - (-1)
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(-1, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);

  // MIN_INT_S21_DECIMAL - 1
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);

  // 3.456 - 1.23 = 2.226
  s21_from_float_to_decimal(3.456f, &dec1);
  s21_from_float_to_decimal(1.23f, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 2.226f, 1e-6);

  // 987654321 - 123456789 = 864197532
  s21_from_int_to_decimal(987654321, &dec1);
  s21_from_int_to_decimal(123456789, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 864197532);

  // 2.000000000 - 0.000000001 = 1.999999999
  s21_from_float_to_decimal(2.000000000f, &dec1);
  s21_from_float_to_decimal(0.000000001f, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 1.999999999f, 1e-6);

  // -1.5 - 1.5 = -3.0
  s21_from_float_to_decimal(-1.5f, &dec1);
  s21_from_float_to_decimal(1.5f, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, -3.0f, 1e-6);

  // MAX_FLOAT_S21_DECIMAL - 1 -> не должно быть переполнения
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(1.0f, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 7.922816e28, 1e+6);

  // MIN_FLOAT_S21_DECIMAL - MIN_FLOAT_S21_DECIMAL = 0
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.0f, 1e-6);

  // 0.000000001 - (-0.000000001) = 0.000000002
  s21_from_float_to_decimal(0.000000001f, &dec1);
  s21_from_float_to_decimal(-0.000000001f, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &res_float), 0);
  ck_assert_float_eq_tol(res_float, 0.000000002f, 1e-6);

  // 9999999999999999999999999999 - (-1)
  s21_from_float_to_decimal(9999999999999999999999999999.0f, &dec1);
  s21_from_int_to_decimal(-1, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);

  // MAX_INT_S21_DECIMAL - MAX_INT_S21_DECIMAL = 0
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // 79228162514264337593543950335 — 0.6 = 79228162514264337593543950334
  dec1 = s21_make_decimal(-1, -1, -1, 0, 0);
  dec2 = s21_make_decimal(6, 0, 0, 0, 1);
  correct_res = s21_make_decimal(-2, -1, -1, 0, 0);
  ck_assert_int_eq(s21_sub(dec1, dec2, &result), 0);
  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(result.bits[i], correct_res.bits[i]);
  }
}
END_TEST
Suite *s21_sub_suite(void) {
  Suite *s = suite_create("s21_sub");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_sub_basic);
  tcase_add_test(tc, test_sub_to_zero);
  tcase_add_test(tc, test_sub_negative_result);
  tcase_add_test(tc, test_sub_with_scale);
  tcase_add_test(tc, test_sub_diff_scale);
  tcase_add_test(tc, test_sub_large_positive);
  tcase_add_test(tc, test_sub_negative_zero);
  tcase_add_test(tc, test_some_test);
  tcase_add_test(tc, test_sub_bank_round);

  suite_add_tcase(s, tc);
  return s;
}
