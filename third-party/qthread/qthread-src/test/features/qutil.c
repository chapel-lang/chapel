#include <float.h>  /* for DBL_EPSILON (according to C89) */
#include <limits.h> /* for INT_MIN & friends (according to C89) */
#include <math.h>   /* for fabs() */
#include <stdio.h>
#include <stdlib.h>

#include "argparsing.h"
#include <qthread/qthread.h>
#include <qthread/qutil.h>

/*
 * This file tests the qutil functions
 *
 */

aligned_t *ui_array;
aligned_t ui_out, ui_sum_authoritative = 0, ui_mult_authoritative = 1,
                  ui_max_authoritative = 0, ui_min_authoritative = UINT_MAX;
size_t ui_len = 10000;
saligned_t *i_array;
saligned_t i_out, i_sum_authoritative = 0, i_mult_authoritative = 1,
                  i_max_authoritative = INT_MIN, i_min_authoritative = INT_MAX;
size_t i_len = 10000;
double *d_array;
double d_out, d_sum_authoritative = 0.0, d_mult_authoritative = 1.0,
              d_max_authoritative = DBL_MIN, d_min_authoritative = DBL_MAX;
size_t d_len = 10000;

static aligned_t qmain(void *junk) {
  size_t i;

  ui_array = (aligned_t *)calloc(ui_len, sizeof(aligned_t));
  iprintf("generating ui_array...\n");
  for (i = 0; i < ui_len; i++) {
    ui_array[i] = random();
    ui_sum_authoritative += ui_array[i];
    ui_mult_authoritative *= ui_array[i];
    if (ui_max_authoritative < ui_array[i]) {
      ui_max_authoritative = ui_array[i];
    }
    if (ui_min_authoritative > ui_array[i]) {
      ui_min_authoritative = ui_array[i];
    }
  }
  iprintf("ui_array generated, calculating sum in parallel..\n");
  ui_out = qutil_uint_sum(ui_array, ui_len, 0);
  test_check(ui_out == ui_sum_authoritative);
  iprintf(" - qutil_uint_sum is correct\n");
  ui_out = qutil_uint_mult(ui_array, ui_len, 0);
  test_check(ui_out == ui_mult_authoritative);
  iprintf(" - qutil_uint_mult is correct\n");
  ui_out = qutil_uint_max(ui_array, ui_len, 0);
  test_check(ui_out == ui_max_authoritative);
  iprintf(" - qutil_uint_max is correct\n");
  ui_out = qutil_uint_min(ui_array, ui_len, 0);
  test_check(ui_out == ui_min_authoritative);
  iprintf(" - qutil_uint_min is correct\n");

  qutil_aligned_qsort(ui_array, ui_len);
  iprintf("done sorting, checking correctness...\n");
  for (i = 0; i < ui_len - 1; i++) {
    if (ui_array[i] > ui_array[i + 1]) {
      /*
       * size_t j;
       *
       * for (j = i-20; j < i+20; j++) {
       * if (j % 8 == 0)
       * printf("\n");
       * printf("[%6u]=%2.5f ", j, d_array[j]);
       * }
       * printf("\n");
       */
      printf("out of order at %lu: %lu > %lu\n",
             (unsigned long)i,
             (unsigned long)ui_array[i],
             (unsigned long)ui_array[i + 1]);
      abort();
    }
  }
  iprintf("%lu\n", (unsigned long)d_len);
  free(ui_array);

  i_array = (saligned_t *)calloc(i_len, sizeof(saligned_t));
  for (i = 0; i < i_len; i++) {
    // Pick most of them to be 1 with a few 2s mixed in.
    // Make the 2s unlikely enough that it's nearly impossible
    // to run into overflow when taking the product.
    if (i_len < 8) {
      i_array[i] = random() % 2 + 1;
    } else {
      i_array[i] = random() < 8 * (INT_MAX / i_len) ? 2 : 1;
    }
    i_sum_authoritative += i_array[i];
    i_mult_authoritative *= i_array[i];
    if (i_max_authoritative < i_array[i]) { i_max_authoritative = i_array[i]; }
    if (i_min_authoritative > i_array[i]) { i_min_authoritative = i_array[i]; }
  }
  iprintf("i_array generated...\n");
  i_out = qutil_int_sum(i_array, i_len, 0);
  test_check(i_out == i_sum_authoritative);
  iprintf(" - qutil_int_sum is correct\n");
  i_out = qutil_int_mult(i_array, i_len, 0);
  test_check(i_out == i_mult_authoritative);
  iprintf(" - qutil_int_mult is correct\n");
  i_out = qutil_int_max(i_array, i_len, 0);
  test_check(i_out == i_max_authoritative);
  iprintf(" - qutil_int_max is correct\n");
  i_out = qutil_int_min(i_array, i_len, 0);
  test_check(i_out == i_min_authoritative);
  free(i_array);
  iprintf(" - qutil_int_min is correct\n");

  d_array = (double *)calloc(d_len, sizeof(double));
  test_check(d_array != NULL);
  for (i = 0; i < d_len; i++) {
    d_array[i] = random() / (double)RAND_MAX * 10;

    d_sum_authoritative += d_array[i];
    d_mult_authoritative *= d_array[i];
    if (d_max_authoritative < d_array[i]) { d_max_authoritative = d_array[i]; }
    if (d_min_authoritative > d_array[i]) { d_min_authoritative = d_array[i]; }
  }
  iprintf("d_array generated...\n");
  d_out = qutil_double_sum(d_array, d_len, 0);
  if (fabs(d_out - d_sum_authoritative) >
      (fabs(d_out + d_sum_authoritative) * FLT_EPSILON)) {
    printf("unexpectedly large sum delta: %g (EPSILON = %g, %g)\n",
           fabs(d_out - d_sum_authoritative),
           (fabs(d_out + d_sum_authoritative) * FLT_EPSILON),
           FLT_EPSILON);
  }
  iprintf(" - qutil_double_sum is correct\n");
  d_out = qutil_double_mult(d_array, d_len, 0);
  if (fabs(d_out - d_mult_authoritative) >
      fabs(d_out + d_mult_authoritative) * FLT_EPSILON) {
    printf("unexpectedly large mult. delta: %g (EPSILON = %g)\n",
           fabs(d_out - d_mult_authoritative),
           fabs(d_out + d_mult_authoritative) * FLT_EPSILON);
  }
  iprintf(" - qutil_double_mult is correct\n");
  d_out = qutil_double_max(d_array, d_len, 0);
  if (fabs(d_out - d_max_authoritative) >
      (fabs(d_out + d_max_authoritative) * DBL_EPSILON)) {
    printf("unexpectedly large max delta: %g (EPSILON = %g)\n",
           fabs(d_out - d_max_authoritative),
           (fabs(d_out + d_max_authoritative) * DBL_EPSILON));
  }
  iprintf(" - qutil_double_max is correct\n");
  d_out = qutil_double_min(d_array, d_len, 0);
  test_check(d_out == d_min_authoritative);
  iprintf(" - qutil_double_min is correct\n");
  /*qutil_mergesort(d_array, d_len, 0);
   * for (i = 0; i < d_len-1; i++) {
   * if (d_array[i] > d_array[i+1]) {
   * printf("out of order at %i: %f > %f\n", i, d_array[i], d_array[i+1]);
   * abort();
   * }
   * } */

  // printf("[qutil] sorting...\n");
  qutil_qsort(d_array, d_len);
  iprintf("done sorting, checking correctness...\n");
  for (i = 0; i < d_len - 1; i++) {
    if (fabs(d_array[i] - d_array[i + 1]) >
        fabs(d_array[i] + d_array[i + 1]) * FLT_EPSILON) {
      // equal
    } else if (d_array[i] > d_array[i + 1]) {
      /*
       * size_t j;
       *
       * for (j = i-20; j < i+20; j++) {
       * if (j % 8 == 0)
       * printf("\n");
       * printf("[%6u]=%2.5f ", j, d_array[j]);
       * }
       * printf("\n");
       */
      printf("out of order at %lu: %g > %g\n",
             (unsigned long)i,
             d_array[i],
             d_array[i + 1]);
      abort();
    }
  }
  free(d_array);
  return 0;
}

int main(int argc, char *argv[]) {
  aligned_t ret;

  test_check(qthread_initialize() == 0);

  CHECK_VERBOSE();
  NUMARG(d_len, "TEST_LEN");
  NUMARG(i_len, "TEST_LEN");
  NUMARG(ui_len, "TEST_LEN");

  test_check(qthread_fork(qmain, NULL, &ret) == 0);
  qthread_readFF(NULL, &ret);
  return 0;
}

/* vim:set expandtab */
