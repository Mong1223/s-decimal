
#include <check.h>

#include "../s21_decimal.h"
#include "../s21_help.h"

START_TEST(test_negate_positive) {
  s21_decimal value = {{123, 0, 0, 0}};
  s21_decimal result;
  int r = s21_negate(value, &result);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal value = {{456, 0, 0, 1u << 31}};
  s21_decimal result;
  int r = s21_negate(value, &result);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(result.bits[0], 456);
  ck_assert_int_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;
  int r = s21_negate(value, &result);
  ck_assert_int_eq(r, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_negate_null_result) {
  s21_decimal value = {{123, 0, 0, 0}};
  int r = s21_negate(value, NULL);
  ck_assert_int_eq(r, 1);
}
END_TEST

START_TEST(test_s21_some_test) {
  s21_decimal value = {0}, result;
  int code;

  ck_assert_int_eq(s21_negate(value, NULL), 1);

  // Негирование положительного числа
  value = s21_make_decimal(123, 0, 0, 0, 0);  // 123
  code = s21_negate(value, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(result.bits[3] & (1U << 31), (1U << 31));  // Проверка знака

  // Негирование отрицательного числа
  value = s21_make_decimal(456, 0, 0, 1, 0);  // -456
  code = s21_negate(value, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 456);
  ck_assert_int_eq(result.bits[3] & (1U << 31),
                   0);  // Должен стать положительным

  // Негирование 0
  value = s21_make_decimal(0, 0, 0, 0, 0);  // 0
  code = s21_negate(value, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(s21_get_sign(result), 1);

  // Негирование числа с дробной частью (123.45 -> -123.45)
  value = s21_make_decimal(12345, 0, 0, 0, 2);  // 123.45
  code = s21_negate(value, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(result.bits[3] & (1U << 31),
                   (1U << 31));  // Должен стать отрицательным

  // Негирование отрицательного числа с дробной частью (-123.45 -> 123.45)
  value = s21_make_decimal(12345, 0, 0, 1, 2);  // -123.45
  code = s21_negate(value, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12345);
  ck_assert_int_eq(result.bits[3] & (1U << 31),
                   0);  // Должен стать положительным

  // Негирование максимального значения
  value = s21_make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);  // MAX
  code = s21_negate(value, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq((unsigned)result.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned)result.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned)result.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[3] & (1U << 31),
                   (1U << 31));  // Должен стать отрицательным

  // Негирование минимального отрицательного значения
  value = s21_make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 1, 0);  // -MAX
  code = s21_negate(value, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq((unsigned)result.bits[0], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned)result.bits[1], 0xFFFFFFFF);
  ck_assert_uint_eq((unsigned)result.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[3] & (1U << 31),
                   0);  // Должен стать положительным
}
END_TEST

Suite *s21_negate_suite(void) {
  Suite *s = suite_create("s21_negate");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_negate_positive);
  tcase_add_test(tc, test_negate_negative);
  tcase_add_test(tc, test_negate_zero);
  tcase_add_test(tc, test_negate_null_result);
  tcase_add_test(tc, test_s21_some_test);

  suite_add_tcase(s, tc);
  return s;
}
