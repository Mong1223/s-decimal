#include <check.h>
#include <stdlib.h>

Suite *from_int_suite(void);
Suite *from_decimal_to_int_suite(void);
Suite *from_float_to_decimal_suite(void);
Suite *from_decimal_to_float_suite(void);

Suite *s21_add_suite(void);
Suite *s21_sub_suite(void);
Suite *s21_mul_suite(void);
Suite *s21_div_suite(void);

Suite *s21_negate_suite(void);
Suite *s21_floor_suite(void);
Suite *s21_truncate_suite(void);
Suite *s21_round_suite(void);

Suite *s21_comparison_suite(void);
int main(void) {
  int failed = 0;
  SRunner *runner = srunner_create(NULL);  // Создаём пустой runner

  // convertor
  srunner_add_suite(runner, from_int_suite());
  srunner_add_suite(runner, from_decimal_to_int_suite());
  srunner_add_suite(runner, from_float_to_decimal_suite());
  srunner_add_suite(runner, from_decimal_to_float_suite());
  // arithmetic
  srunner_add_suite(runner, s21_add_suite());
  srunner_add_suite(runner, s21_sub_suite());
  srunner_add_suite(runner, s21_mul_suite());
  srunner_add_suite(runner, s21_div_suite());
  // other
  srunner_add_suite(runner, s21_negate_suite());
  srunner_add_suite(runner, s21_floor_suite());
  srunner_add_suite(runner, s21_truncate_suite());
  srunner_add_suite(runner, s21_round_suite());
  // comparison
  srunner_add_suite(runner, s21_comparison_suite());

  // Запускаем все тесты
  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}