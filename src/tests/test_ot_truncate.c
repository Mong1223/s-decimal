#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
START_TEST(test_truncate_positive_fraction) {
  s21_decimal val = s21_make_decimal(199, 0, 0, 0, 2);  // 1.99
  s21_decimal result;
  s21_truncate(val, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_truncate_negative_fraction) {
  s21_decimal val = s21_make_decimal(199, 0, 0, 1, 2);  // -1.99
  s21_decimal result;
  s21_truncate(val, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_truncate_zero) {
  s21_decimal val = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_truncate(val, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_truncate_positive_integer) {
  s21_decimal val = {{123456, 0, 0, 0}};
  s21_decimal result;
  s21_truncate(val, &result);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_truncate_negative_integer) {
  s21_decimal val = {{123456, 0, 0, 1u << 31}};
  s21_decimal result;
  s21_truncate(val, &result);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

Suite *s21_truncate_suite(void) {
  Suite *s = suite_create("s21_truncate");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_truncate_positive_fraction);
  tcase_add_test(tc, test_truncate_negative_fraction);
  tcase_add_test(tc, test_truncate_zero);
  tcase_add_test(tc, test_truncate_positive_integer);
  tcase_add_test(tc, test_truncate_negative_integer);

  suite_add_tcase(s, tc);
  return s;
}