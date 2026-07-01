#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"
// lo, scale, sign
START_TEST(test_compare_all) {
  s21_decimal a = s21_make_decimal(100, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(200, 0, 0, 0, 0);
  s21_decimal c = s21_make_decimal(100, 0, 0, 0, 0);
  s21_decimal d = s21_make_decimal(100, 0, 0, 1, 0);  // -100
  s21_decimal zero_p = s21_make_decimal(0, 0, 0, 0, 0);
  s21_decimal zero_n = s21_make_decimal(0, 0, 0, 1, 0);

  // Equal / Not Equal
  ck_assert_int_eq(s21_is_equal(a, c), 1);
  ck_assert_int_eq(s21_is_equal(zero_p, zero_n), 1);
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
  ck_assert_int_eq(s21_is_not_equal(a, c), 0);

  // Less / Greater
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(b, a), 0);
  ck_assert_int_eq(s21_is_greater(b, a), 1);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
  ck_assert_int_eq(s21_is_less(d, a), 1);
  ck_assert_int_eq(s21_is_greater(a, d), 1);

  // Less or Equal / Greater or Equal
  ck_assert_int_eq(s21_is_less_or_equal(a, c), 1);
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(b, a), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(a, c), 1);
  ck_assert_int_eq(s21_is_less_or_equal(d, zero_p), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(zero_p, d), 1);
}
END_TEST

START_TEST(test_s21_is_less) {
  s21_decimal dec1, dec2;

  // -0 < 0
  s21_from_int_to_decimal(-0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // -0.0 < 0.0
  s21_from_float_to_decimal(-0.0f, &dec1);
  s21_from_float_to_decimal(0.0f, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // 5 < 10
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(10, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // 10 < 5
  s21_from_int_to_decimal(10, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // -5 < 0
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // 0 < -5
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // -10 < -5
  s21_from_int_to_decimal(-10, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // -5 < -10
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-10, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // 5 < 5
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // -5 < -5
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // 0.1 < 0.2
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // 0.2 < 0.1
  s21_from_float_to_decimal(0.2f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // -0.1 < 0.1
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // 0.000001 < 0.000002
  s21_from_float_to_decimal(0.000001f, &dec1);
  s21_from_float_to_decimal(0.000002f, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // 1000000 < 1000001
  s21_from_int_to_decimal(1000000, &dec1);
  s21_from_int_to_decimal(1000001, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // INT_MAX < INT_MAX
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // INT_MIN < INT_MAX
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);

  // MAX_DECIMAL_FLOAT < MIN_DECIMAL_FLOAT
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 0);

  // MIN_DECIMAL_FLOAT < MAX_DECIMAL_FLOAT
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_less(dec1, dec2), 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal) {
  s21_decimal dec1, dec2;

  // -0 <= 0
  s21_from_int_to_decimal(-0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // 5 <= 10
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(10, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // 10 <= 5
  s21_from_int_to_decimal(10, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 0);

  // -5 <= 0
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // 0 <= -5
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 0);

  // -10 <= -5
  s21_from_int_to_decimal(-10, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // -5 <= -10
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-10, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 0);

  // 5 <= 5
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // -5 <= -5
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // 0.1 <= 0.2
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // 0.2 <= 0.1
  s21_from_float_to_decimal(0.2f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 0);

  // -0.1 <= 0.1
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // 0.000001 <= 0.000002
  s21_from_float_to_decimal(0.000001f, &dec1);
  s21_from_float_to_decimal(0.000002f, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // 1000000 <= 1000001
  s21_from_int_to_decimal(1000000, &dec1);
  s21_from_int_to_decimal(1000001, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // INT_MAX <= INT_MAX
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // INT_MIN <= INT_MAX
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);

  // MAX_DECIMAL_FLOAT <= MIN_DECIMAL_FLOAT
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 0);

  // MIN_DECIMAL_FLOAT <= MAX_DECIMAL_FLOAT
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_less_or_equal(dec1, dec2), 1);
}
END_TEST

START_TEST(test_s21_is_greater) {
  s21_decimal dec1, dec2;

  // -0 > 0
  s21_from_int_to_decimal(-0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);

  // 5 > 1
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // 1 > 5
  s21_from_int_to_decimal(1, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);

  // -5 > -10
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-10, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // -10 > -5
  s21_from_int_to_decimal(-10, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);

  // 5 > 5
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);

  // -5 > -5
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);

  // 0.2 > 0.1
  s21_from_float_to_decimal(0.2f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // 0.1 > 0.2
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);

  // -0.1 > -0.2
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(-0.2f, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // 0.000002 > 0.000001
  s21_from_float_to_decimal(0.000002f, &dec1);
  s21_from_float_to_decimal(0.000001f, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // 1000001 > 1000000
  s21_from_int_to_decimal(1000001, &dec1);
  s21_from_int_to_decimal(1000000, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // INT_MAX > INT_MIN
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MIN, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // INT_MIN > INT_MAX
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);

  // MAX_DECIMAL_FLOAT > MIN_DECIMAL_FLOAT
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 1);

  // MIN_DECIMAL_FLOAT > MAX_DECIMAL_FLOAT
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_greater(dec1, dec2), 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal) {
  s21_decimal dec1, dec2;

  // -0 >= 0
  s21_from_int_to_decimal(-0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // 5 >= 1
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // 1 >= 5
  s21_from_int_to_decimal(1, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 0);

  // -5 >= -10
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-10, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // -10 >= -5
  s21_from_int_to_decimal(-10, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 0);

  // 5 >= 5
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // -5 >= -5
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // 0.2 >= 0.1
  s21_from_float_to_decimal(0.2f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // 0.1 >= 0.2
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 0);

  // -0.1 >= -0.2
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(-0.2f, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // 0.000002 >= 0.000001
  s21_from_float_to_decimal(0.000002f, &dec1);
  s21_from_float_to_decimal(0.000001f, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // 1000001 >= 1000000
  s21_from_int_to_decimal(1000001, &dec1);
  s21_from_int_to_decimal(1000000, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // INT_MAX >= INT_MIN
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MIN, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // INT_MIN >= INT_MAX
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 0);

  // MAX_DECIMAL_FLOAT >= MIN_DECIMAL_FLOAT
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 1);

  // MIN_DECIMAL_FLOAT >= MAX_DECIMAL_FLOAT
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec1, dec2), 0);
}
END_TEST

START_TEST(test_s21_is_equal) {
  s21_decimal dec1, dec2;

  // -0 == 0
  s21_from_int_to_decimal(-0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);

  // -0.0 == 0.0
  s21_from_float_to_decimal(-0.0f, &dec1);
  s21_from_float_to_decimal(0.0f, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);

  // 5 == 5
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);

  // 5 == 6
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(6, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 0);

  // -5 == 5
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 0);

  // -5 == -6
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-6, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 0);

  // 0 == 0
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);

  // 0 == 1
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 0);

  // -0.1 == 0.1
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 0);

  // 0.1 == 0.2
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 0);

  // -0.1 == -0.1
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(-0.1f, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);

  // -0.1 == -0.2
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(-0.2f, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 0);

  // 1000000 == 1000000
  s21_from_int_to_decimal(1000000, &dec1);
  s21_from_int_to_decimal(1000000, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);

  // INT_MAX == INT_MAX
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);
}
END_TEST

START_TEST(test_eq_same_value) {
  s21_decimal a = s21_make_decimal(12345, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(12345, 0, 0, 0, 0);
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_eq_diff_scale) {
  s21_decimal a = s21_make_decimal(100, 0, 0, 0, 2);  // 1.00
  s21_decimal b = s21_make_decimal(10, 0, 0, 0, 1);   // 1.0
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_eq_zero_positive_and_negative) {
  s21_decimal a = s21_make_decimal(0, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(0, 0, 0, 1, 0);
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_eq_not_equal) {
  s21_decimal a = s21_make_decimal(1000, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(1001, 0, 0, 0, 0);
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_eq_diff_sign) {
  s21_decimal a = s21_make_decimal(42, 0, 0, 0, 0);
  s21_decimal b = s21_make_decimal(42, 0, 0, 1, 0);
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_not_equal) {
  s21_decimal dec1, dec2;

  // -0 != 0
  s21_from_int_to_decimal(-0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // 5 != 6
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(6, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // 5 != 5
  s21_from_int_to_decimal(5, &dec1);
  s21_from_int_to_decimal(5, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // -5 != -6
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-6, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // -5 != -5
  s21_from_int_to_decimal(-5, &dec1);
  s21_from_int_to_decimal(-5, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // 0 != 1
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(1, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // 0 != 0
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // 0.1 != 0.2
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.2f, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // 0.1 != 0.1
  s21_from_float_to_decimal(0.1f, &dec1);
  s21_from_float_to_decimal(0.1f, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // -0.1 != -0.2
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(-0.2f, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // -0.1 != -0.1
  s21_from_float_to_decimal(-0.1f, &dec1);
  s21_from_float_to_decimal(-0.1f, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // 1000000 != 1000001
  s21_from_int_to_decimal(1000000, &dec1);
  s21_from_int_to_decimal(1000001, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // INT_MAX != INT_MIN
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MIN, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // MAX_DECIMAL_FLOAT != MIN_DECIMAL_FLOAT
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 1);

  // INT_MAX != INT_MAX
  s21_from_int_to_decimal(INT_MAX, &dec1);
  s21_from_int_to_decimal(INT_MAX, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // INT_MIN != INT_MIN
  s21_from_int_to_decimal(INT_MIN, &dec1);
  s21_from_int_to_decimal(INT_MIN, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // MAX_DECIMAL_FLOAT != MAX_DECIMAL_FLOAT
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MAX_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);

  // MIN_DECIMAL_FLOAT != MIN_DECIMAL_FLOAT
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec1);
  s21_from_float_to_decimal(MIN_DECIMAL_FLOAT, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);
}
END_TEST

Suite *s21_comparison_suite(void) {
  Suite *s = suite_create("s21_comparison");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_compare_all);
  tcase_add_test(tc, test_s21_is_less);
  tcase_add_test(tc, test_s21_is_less_or_equal);
  tcase_add_test(tc, test_s21_is_greater);
  tcase_add_test(tc, test_s21_is_greater_or_equal);
  tcase_add_test(tc, test_s21_is_equal);
  tcase_add_test(tc, test_eq_same_value);
  tcase_add_test(tc, test_eq_diff_scale);
  tcase_add_test(tc, test_eq_zero_positive_and_negative);
  tcase_add_test(tc, test_eq_not_equal);
  tcase_add_test(tc, test_eq_diff_sign);
  tcase_add_test(tc, test_s21_is_not_equal);

  suite_add_tcase(s, tc);
  return s;
}