#ifndef QTHREAD_QUTIL_H
#define QTHREAD_QUTIL_H

#include "qthread.h"

Q_STARTCXX /* */

  /* This computes the sum/product of all the doubles in an array. If checkfeb
   * is non-zero, then it will wait for each array entry to be marked FEB-full
   */
  double
  qutil_double_sum(double const *array, size_t length, int checkfeb);
double qutil_double_mult(double const *array, size_t length, int checkfeb);
double qutil_double_max(double const *array, size_t length, int checkfeb);
double qutil_double_min(double const *array, size_t length, int checkfeb);
/* This computes the sum/product of all the aligned_ts in an array */
aligned_t qutil_uint_sum(aligned_t const *array, size_t length, int checkfeb);
aligned_t qutil_uint_mult(aligned_t const *array, size_t length, int checkfeb);
aligned_t qutil_uint_max(aligned_t const *array, size_t length, int checkfeb);
aligned_t qutil_uint_min(aligned_t const *array, size_t length, int checkfeb);
/* This computes the sum/product of all the saligned_ts in an array */
saligned_t qutil_int_sum(saligned_t const *array, size_t length, int checkfeb);
saligned_t qutil_int_mult(saligned_t const *array, size_t length, int checkfeb);
saligned_t qutil_int_max(saligned_t const *array, size_t length, int checkfeb);
saligned_t qutil_int_min(saligned_t const *array, size_t length, int checkfeb);

void qutil_mergesort(double *array, size_t length);
void qutil_qsort(double *array, size_t length);
void qutil_aligned_qsort(aligned_t *array, size_t length);

Q_ENDCXX /* */
#endif   // ifndef QTHREAD_QUTIL_H
  /* vim:set expandtab: */
