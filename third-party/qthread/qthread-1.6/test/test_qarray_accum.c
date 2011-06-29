#include <qthread/qthread.h>
#include <qthread/qarray.h>

#include "argparsing.h"

#include <assert.h>

static double ret = 0.0;

static void assigni(const size_t startat,
                    const size_t stopat,
                    qarray * q,
                    void *arg)
{
    int *ptr = (int *)qarray_elem(q, startat);
    size_t i;

    for (i = 0; i < (stopat - startat); ++i) {
        ptr[i] = (int)(i + startat);
    }
}

static void permute(const size_t startat,
                    const size_t stopat,
                    qarray * q,
                    void *arg,
                    void *ret)
{
    int *ptr = (int *)qarray_elem(q, startat);
    size_t i;
    double sum = 0.0;

    for (i = 0; i < (stopat - startat); ++i) {
        double tmp = (double)ptr[i];
        qt_dbl_add_acc(&sum, &tmp);
    }
    memcpy(ret, &sum, sizeof(double));
}

static aligned_t onesum(void *arg)
{
    qthread_dincr(&ret, (double)*(int *)arg);
    return 0;
}

int main(int argc,
         char *argv[])
{
    qarray *t;
    size_t int_calc = 0, i;
    size_t ITER = 10000;

    qthread_initialize();
    CHECK_VERBOSE();
    NUMARG(ITER, "ITERATIONS");

    t = qarray_create_tight(ITER, sizeof(int));
    assert(t);
    qarray_iter_loop(t, 0, ITER, assigni, NULL);
    for (i = 1; i < ITER; i++) {
        int_calc += i;
    }
    /* ******************************
     * Example 1
     */
    qarray_iter_loopaccum(t, 0, ITER, permute, NULL, &ret, sizeof(double),
                          qt_dbl_add_acc);
    iprintf("int = %lu\n", (long unsigned)int_calc);
    iprintf("ret = %f\n", ret);
    assert(int_calc == ret);
    ret = 0.0;
    /* ******************************
     * Example 2
     */
    qarray_iter(t, 0, ITER, onesum);
    iprintf("int = %lu\n", (long unsigned)int_calc);
    iprintf("ret = %f\n", ret);
    assert(int_calc == ret);
    qarray_destroy(t);
    return 0;
}

/* vim:set expandtab */
