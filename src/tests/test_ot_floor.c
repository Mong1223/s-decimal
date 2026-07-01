#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
START_TEST(test_floor_positive_fraction) {
  s21_decimal val = s21_make_decimal(95, 0, 0, 0, 2);
  s21_decimal result;
  s21_floor(val, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_floor_negative_fraction) {
  s21_decimal val = s21_make_decimal(95, 0, 0, 1, 2);
  s21_decimal result;
  s21_floor(val, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal val = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_floor(val, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_floor_positive_integer) {
  s21_decimal val = {{42, 0, 0, 0}};
  s21_decimal result;
  s21_floor(val, &result);
  ck_assert_int_eq(result.bits[0], 42);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_floor_negative_integer) {
  s21_decimal val = {{42, 0, 0, 1u << 31}};
  s21_decimal result;
  s21_floor(val, &result);
  ck_assert_int_eq(result.bits[0], 42);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_some_test) {
  s21_decimal dec = {0}, result;
  int res_int;

  ck_assert_int_eq(s21_floor(dec, NULL), 1);

  // 5.9 -> 5
  s21_from_float_to_decimal(5.9f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 5);

  // -5.9 -> -6
  s21_from_float_to_decimal(-5.9f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -6);

  // 3.1 -> 3
  s21_from_float_to_decimal(3.1f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 3);

  // -3.1 -> -4
  s21_from_float_to_decimal(-3.1f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -4);

  // 0.9 -> 0
  s21_from_float_to_decimal(0.9f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // -0.9 -> -1
  s21_from_float_to_decimal(-0.9f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -1);

  // 0.0 -> 0
  s21_from_float_to_decimal(0.0f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // -0.0 -> 0
  s21_from_float_to_decimal(-0.0f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 0);

  // 10.9999 -> 10
  s21_from_float_to_decimal(10.9999f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, 10);

  // -10.9999 -> -11
  s21_from_float_to_decimal(-10.9999f, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, -11);

  // MAX_INT_S21_DECIMAL -> MAX_INT_S21_DECIMAL
  s21_from_int_to_decimal(INT_MAX, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
  ck_assert_int_eq(res_int, INT_MAX);

  // MIN_INT_S21_DECIMAL -> MIN_INT_S21_DECIMAL
  s21_from_int_to_decimal(INT_MIN, &dec);
  ck_assert_int_eq(s21_floor(dec, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_int(result, &res_int), 0);
}
END_TEST

Suite *s21_floor_suite(void) {
  Suite *s = suite_create("s21_floor");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_floor_positive_fraction);
  tcase_add_test(tc, test_floor_negative_fraction);
  tcase_add_test(tc, test_floor_zero);
  tcase_add_test(tc, test_floor_positive_integer);
  tcase_add_test(tc, test_floor_negative_integer);
  tcase_add_test(tc, test_s21_some_test);

  suite_add_tcase(s, tc);
  return s;
}