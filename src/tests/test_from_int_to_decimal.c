#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
// Нулевое значение
START_TEST(test_zero) {
  s21_decimal d = {0};
  int result = s21_from_int_to_decimal(0, &d);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(d.bits[0], 0);
  ck_assert_int_eq(s21_get_sign(d), 0);
  ck_assert_int_eq(s21_get_scale(d), 0);
}
END_TEST

// Минимальное положительное
START_TEST(test_positive_one) {
  s21_decimal d = {0};
  int result = s21_from_int_to_decimal(1, &d);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(d.bits[0], 1);
  ck_assert_int_eq(s21_get_sign(d), 0);
  ck_assert_int_eq(s21_get_scale(d), 0);
}
END_TEST

// Максимальное значение int
START_TEST(test_int_max) {
  s21_decimal d = {0};
  int result = s21_from_int_to_decimal(2147483647, &d);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(d.bits[0], 2147483647);
  ck_assert_int_eq(s21_get_sign(d), 0);
  ck_assert_int_eq(s21_get_scale(d), 0);
}
END_TEST

// Минимальное значение int
START_TEST(test_int_min) {
  s21_decimal d = {0};
  int result = s21_from_int_to_decimal(-2147483648, &d);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(d.bits[0], 2147483648u);  // беззнаковое значение
  ck_assert_int_eq(s21_get_sign(d), 1);
  ck_assert_int_eq(s21_get_scale(d), 0);
}
END_TEST

// Произвольное положительное
START_TEST(test_positive) {
  s21_decimal d = {0};
  int result = s21_from_int_to_decimal(987654, &d);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(d.bits[0], 987654);
  ck_assert_int_eq(s21_get_sign(d), 0);
  ck_assert_int_eq(s21_get_scale(d), 0);
}
END_TEST

// Произвольное отрицательное
START_TEST(test_negative) {
  s21_decimal d = {0};
  int result = s21_from_int_to_decimal(-123456, &d);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(d.bits[0], 123456);
  ck_assert_int_eq(s21_get_sign(d), 1);
  ck_assert_int_eq(s21_get_scale(d), 0);
}
END_TEST

// NULL указатель
START_TEST(test_null_ptr) {
  int result = s21_from_int_to_decimal(123, NULL);
  ck_assert_int_eq(result, 1);
}
END_TEST

// Проверка, что верхние биты очищены
START_TEST(test_clear_upper_bits) {
  s21_decimal d;
  d.bits[0] = d.bits[1] = d.bits[2] = d.bits[3] = 0xFFFFFFFF;
  int result = s21_from_int_to_decimal(42, &d);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(d.bits[0], 42);
  ck_assert_int_eq(d.bits[1], 0);
  ck_assert_int_eq(d.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(d), 0);
  ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST
/// sometest
START_TEST(test_s21_some_test) {
  s21_decimal dst;
  int src = 0, err;

  err = s21_from_int_to_decimal(src, NULL);
  ck_assert_int_eq(err, 1);

  src = -0;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);

  src = 12345;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);

  src = -12345;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], -src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], (unsigned)INT_MIN);

  src = INT_MAX;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);

  src = INT_MIN;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], (unsigned)src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], (unsigned)INT_MIN);

  src = 0;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);

  src = 1;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);

  src = -1;
  err = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dst.bits[0], -src);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], (unsigned)INT_MIN);

  src = 12345;
  err = s21_from_int_to_decimal(src, NULL);
  ck_assert_int_eq(err, 1);
}
END_TEST
///
Suite *from_int_suite(void) {
  Suite *s = suite_create("from_int_to_decimal");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_zero);
  tcase_add_test(tc, test_positive_one);
  tcase_add_test(tc, test_int_max);
  tcase_add_test(tc, test_int_min);
  tcase_add_test(tc, test_positive);
  tcase_add_test(tc, test_negative);
  tcase_add_test(tc, test_null_ptr);
  tcase_add_test(tc, test_clear_upper_bits);
  tcase_add_test(tc, test_s21_some_test);
  suite_add_tcase(s, tc);
  return s;
}
