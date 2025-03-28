#include "argparsing.h"
#include <float.h>  /* for DBL_MIN & friends (according to C89) */
#include <limits.h> /* for INT_MIN & friends (according to C89) */
#include <qthread/qloop.h>
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// #define BIGLEN 200000000U
static unsigned int BIGLEN = 1000000U;

int main(int argc, char *argv[]) {
  size_t i;
  struct timeval start, stop;

  test_check(qthread_initialize() == QTHREAD_SUCCESS);
  CHECK_VERBOSE();
  NUMARG(BIGLEN, "BIGLEN");

  {
    aligned_t *uia, uitmp, uisum = 0, uiprod = 1, uimax = 0, uimin = UINT_MAX;

    uia = (aligned_t *)malloc(sizeof(aligned_t) * BIGLEN);
    test_check(uia);
    for (i = 0; i < BIGLEN; i++) { uia[i] = random(); }
    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++) uisum += uia[i];
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] summing-serial   %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    uitmp = qt_uint_sum(uia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] summing-parallel %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(uitmp == uisum);

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++) uiprod *= uia[i];
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] multiplying-serial   %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    uitmp = qt_uint_prod(uia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] multiplying-parallel %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(uitmp == uiprod);

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++)
      if (uia[i] > uimax) { uimax = uia[i]; }
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmax-serial   %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    uitmp = qt_uint_max(uia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmax-parallel %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(uimax == uitmp);

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++)
      if (uia[i] < uimin) { uimin = uia[i]; }
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmin-serial   %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    uitmp = qt_uint_min(uia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmin-parallel %u uints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(uitmp == uimin);
    free(uia);
  }

  {
    saligned_t *ia, itmp, isum = 0, iprod = 1, imax = INT_MIN, imin = INT_MAX;

    ia = (saligned_t *)malloc(sizeof(saligned_t) * BIGLEN);
    test_check(ia);
    for (i = 0; i < BIGLEN; i++) {
      // Pick most of them to be 1 with a few 2s mixed in.
      // Make the 2s unlikely enough that it's nearly impossible
      // to run into overflow when taking the product.
      if (BIGLEN < 8) {
        ia[i] = random() % 2 + 1;
      } else {
        ia[i] = random() < 8 * (INT_MAX / BIGLEN) ? 2 : 1;
      }
    }
    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++) isum += ia[i];
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] summing-serial   %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    itmp = qt_int_sum(ia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] summing-parallel %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(itmp == isum);

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++) iprod *= ia[i];
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] multiplying-serial   %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    itmp = qt_int_prod(ia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] multiplying-parallel %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(itmp == iprod);

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++)
      if (ia[i] > imax) { imax = ia[i]; }
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmax-serial   %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    itmp = qt_int_max(ia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmax-parallel %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(imax == itmp);

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++)
      if (ia[i] < imin) { imin = ia[i]; }
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmin-serial   %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    itmp = qt_int_min(ia, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmin-parallel %u ints took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(itmp == imin);
    free(ia);
  }

  {
    double *da, dtmp, dsum = 0.0, dprod = 1.0, dmin = DBL_MAX, dmax = DBL_MIN;

    da = (double *)malloc(sizeof(double) * BIGLEN);
    test_check(da);
    srandom(0xdeadbeef);
    for (i = 0; i < BIGLEN; i++) { da[i] = random() / (double)RAND_MAX * 10.0; }
    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++) dsum += da[i];
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] summing-serial   %u doubles took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(dsum > 0);
    gettimeofday(&start, NULL);
    dtmp = qt_double_sum(da, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] summing-parallel %u doubles took %g second\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(dtmp > 0);
    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++) dprod *= da[i];
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] multiplying-serial   %u doubles took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(dprod > 0);
    gettimeofday(&start, NULL);
    dtmp = qt_double_prod(da, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] multiplying-parallel %u doubles took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(dtmp > 0);
    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++)
      if (da[i] > dmax) { dmax = da[i]; }
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmax-serial   %u doubles took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    dtmp = qt_double_max(da, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmax-parallel %u doubles took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(dmax == dtmp);

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGLEN; i++)
      if (da[i] < dmin) { dmin = da[i]; }
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmin-serial   %u doubles took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    gettimeofday(&start, NULL);
    dtmp = qt_double_min(da, BIGLEN, 0);
    gettimeofday(&stop, NULL);
    iprintf("[test_qloop] findmin-parallel %u doubles took %g seconds\n",
            BIGLEN,
            (stop.tv_sec + (stop.tv_usec * 1.0e-6)) -
              (start.tv_sec + (start.tv_usec * 1.0e-6)));
    test_check(dtmp == dmin);
    free(da);
  }

  return 0;
}

/* vim:set expandtab */
