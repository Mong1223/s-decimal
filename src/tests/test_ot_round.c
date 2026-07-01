#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
START_TEST(test_round_positive_fraction_down) {
  // s21_decimal val = MAKE_DEC(24, 1, 0);               // 2.4
  s21_decimal val = s21_make_decimal(24, 0, 0, 0, 1);  // 2.4
  s21_decimal result;
  s21_round(val, &result);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_round_positive_fraction_up) {
  // s21_decimal val = MAKE_DEC(26, 1, 0); // 2.6
  s21_decimal val = s21_make_decimal(26, 0, 0, 0, 1);
  s21_decimal result;
  s21_round(val, &result);
  ck_assert_int_eq(result.bits[0], 3);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_round_negative_fraction_down) {
  // s21_decimal val = MAKE_DEC(24, 1, 1); // -2.4
  s21_decimal val = s21_make_decimal(24, 0, 0, 1, 1);
  s21_decimal result;
  s21_round(val, &result);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_round_negative_fraction_up) {
  s21_decimal val = s21_make_decimal(26, 0, 0, 1, 1);
  s21_decimal result;
  s21_round(val, &result);
  ck_assert_int_eq(result.bits[0], 3);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_round_zero) {
  // s21_decimal val = MAKE_DEC(0, 5, 0); // 0.00000
  s21_decimal val = s21_make_decimal(0, 0, 0, 0, 5);
  s21_decimal result;
  s21_round(val, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_round_integer) {
  s21_decimal val = s21_make_decimal(1234, 0, 0, 0, 0);
  s21_decimal result;
  s21_round(val, &result);
  ck_assert_int_eq(result.bits[0], 1234);
  ck_assert_int_eq(s21_get_scale(result), 0);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

Suite *s21_round_suite(void) {
  Suite *s = suite_create("s21_round");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_round_positive_fraction_down);
  tcase_add_test(tc, test_round_positive_fraction_up);
  tcase_add_test(tc, test_round_negative_fraction_down);
  tcase_add_test(tc, test_round_negative_fraction_up);
  tcase_add_test(tc, test_round_zero);
  tcase_add_test(tc, test_round_integer);

  suite_add_tcase(s, tc);
  return s;
}
