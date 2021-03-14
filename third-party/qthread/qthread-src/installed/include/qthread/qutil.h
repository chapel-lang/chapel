#ifndef QTHREAD_QUTIL_H
#define QTHREAD_QUTIL_H

#include "qthread.h"

Q_STARTCXX /* */

/* This computes the sum/product of all the doubles in an array. If checkfeb is
 * non-zero, then it will wait for each array entry to be marked FEB-full */
double qutil_double_sum(const double *array,
                        size_t        length,
                        int           checkfeb);
double qutil_double_mult(const double *array,
                         size_t        length,
                         int           checkfeb);
double qutil_double_max(const double *array,
                        size_t        length,
                        int           checkfeb);
double qutil_double_min(const double *array,
                        size_t        length,
                        int           checkfeb);
/* This computes the sum/product of all the aligned_ts in an array */
aligned_t qutil_uint_sum(const aligned_t *array,
                         size_t           length,
                         int              checkfeb);
aligned_t qutil_uint_mult(const aligned_t *array,
                          size_t           length,
                          int              checkfeb);
aligned_t qutil_uint_max(const aligned_t *array,
                         size_t           length,
                         int              checkfeb);
aligned_t qutil_uint_min(const aligned_t *array,
                         size_t           length,
                         int              checkfeb);
/* This computes the sum/product of all the saligned_ts in an array */
saligned_t qutil_int_sum(const saligned_t *array,
                         size_t            length,
                         int               checkfeb);
saligned_t qutil_int_mult(const saligned_t *array,
                          size_t            length,
                          int               checkfeb);
saligned_t qutil_int_max(const saligned_t *array,
                         size_t            length,
                         int               checkfeb);
saligned_t qutil_int_min(const saligned_t *array,
                         size_t            length,
                         int               checkfeb);

void qutil_mergesort(double *array,
                     size_t  length);
void qutil_qsort(double *array,
                 size_t  length);
void qutil_aligned_qsort(aligned_t *array,
                         size_t     length);

Q_ENDCXX /* */
#endif // ifndef QTHREAD_QUTIL_H
/* vim:set expandtab: */
