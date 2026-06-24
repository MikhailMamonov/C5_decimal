#include <check.h>
#include <stdlib.h>

#include "../s21_decimal.h"
// standart library functions
Suite *add_suite_create(void);
Suite *sub_suite_create(void);
Suite *mul_suite_create(void);
Suite *div_suite_create(void);


void assemble_srunner(SRunner *sr) {
  srunner_add_suite(sr, add_suite_create());
  srunner_add_suite(sr, sub_suite_create());
  srunner_add_suite(sr, mul_suite_create());
  srunner_add_suite(sr, div_suite_create());
}

int main(void) {
  int failed = 0;
  SRunner *sr = srunner_create();
  assemble_srunner(sr);

  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

