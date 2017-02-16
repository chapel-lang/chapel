#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <float.h> // for DBL_MAX, from C89
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

typedef struct {
    double max;
    double min;
    double sum;
    double prod;
    double avg;
    size_t cnt;
} multiple_op_t;

static aligned_t numincrs       = 1024;
static double   *random_numbers = NULL;
static size_t   *sleeplen       = NULL;
static size_t    numiters       = 10;
static qtimer_t  timer;

static multiple_op_t reset = { 0, DBL_MAX, 0, 1, 0, 0 };
static multiple_op_t known_correct = { 0, DBL_MAX, 0, 1, 0, 0 };

static void count(const size_t startat,
                  const size_t stopat,
                  void        *arg_,
                  void        *ret_)
{
    multiple_op_t ret;

    ret.max  = random_numbers[startat];
    ret.min  = random_numbers[startat];
    ret.sum  = random_numbers[startat];
    ret.prod = random_numbers[startat];
    ret.avg  = random_numbers[startat];
    ret.cnt  = 1;
    usleep(sleeplen[startat]);
    for (size_t i = startat + 1; i < stopat; ++i) {
        ret.max   = (random_numbers[i] > ret.max) ? random_numbers[i] : ret.max;
        ret.min   = (random_numbers[i] > ret.min) ? ret.min : random_numbers[i];
        ret.sum  += random_numbers[i];
        ret.prod *= random_numbers[i];
        ret.avg   = ((ret.avg * ret.cnt) / (ret.cnt + 1)) + (random_numbers[i] / (ret.cnt + 1));
        ret.cnt++;
        usleep(sleeplen[i]);
    }
    *(multiple_op_t *)ret_ = ret;
}

static void multi_op(void *restrict       a_,
                     const void *restrict b_)
{
    multiple_op_t *restrict a = (multiple_op_t *)a_;
    multiple_op_t *restrict b = (multiple_op_t *)b_;

    // printf("a->max = %f, b->max = %f\n", a->max, b->max);
    a->max = (a->max > b->max) ? a->max : b->max;
    // printf("a->max = %f\n", a->max);
    // printf("a->min = %f, b->min = %f\n", a->min, b->min);
    a->min = (a->min > b->min) ? b->min : a->min;
    // printf("a->min = %f\n", a->min);
    // printf("a->sum = %f, b->sum = %f\n", a->sum, b->sum);
    a->sum += b->sum;
    // printf("a->sum = %f\n", a->sum);
    // printf("a->prod = %f, b->prod = %f\n", a->prod, b->prod);
    a->prod *= b->prod;
    // printf("a->prod = %f\n", a->prod);
    // printf("a->avg = %f, b->avg = %f\n", a->avg, b->avg);
    if (b->cnt > 0) {
        if (a->cnt > 0) {
            a->avg = ((a->avg * a->cnt) / (a->cnt + b->cnt)) + ((b->avg * b->cnt) / (a->cnt + b->cnt));
        } else {
            a->avg = b->avg;
        }
    }
    // printf("a->avg = %f\n", a->avg);
    // printf("a->cnt = %zu, b->cnt = %zu\n", a->cnt, b->cnt);
    a->cnt += b->cnt;
    // printf("a->cnt = %zu\n", a->cnt);
}

typedef void (*qtloopaccum_f)(const size_t     a,
                              const size_t     b,
                              const size_t     sz,
                              void            *out,
                              const qt_loopr_f f,
                              void            *c,
                              const qt_accum_f acc);

static void run_iterations(qtloopaccum_f loop,
                           qt_loopr_f    func,
                           qt_accum_f    acc,
                           double        overhead,
                           const char   *type,
                           const char   *name)
{
    double        total  = 0;
    int           passed = 0;
    int           i;
    multiple_op_t tmp = reset;

    // to warm the caches
    loop(0, numincrs, sizeof(tmp), &tmp, func, NULL, acc);
    // now, to time it
    for (i = 0; i < numiters; ++i) {
        tmp = reset;
        qtimer_start(timer);
        loop(0, numincrs, sizeof(tmp), &tmp, func, NULL, acc);
        qtimer_stop(timer);
        // printf("max = %f (%f)\n", tmp.max, known_correct.max);
        passed = (tmp.max == known_correct.max);
        if (!passed) { break; }
        // printf("min = %f (%f)\n", tmp.min, known_correct.min);
        passed = (tmp.min == known_correct.min);
        if (!passed) { break; }
        // printf("sum = %f (%f)\n", tmp.sum, known_correct.sum);
        // printf("prd = %f (%f)\n", tmp.prod, known_correct.prod);
        // printf("avg = %f (%f)\n", tmp.avg, known_correct.avg);
        // printf("cnt = %zu (%zu)\n", tmp.cnt, known_correct.cnt);
        passed = (tmp.cnt == known_correct.cnt);
        if (!passed) { break; }
        total += qtimer_secs(timer);
        if (!passed) {
            break;
        }
    }
    printf("%-21s %-9s %5lu %8lu %8f %s\n", type, name, (unsigned long)numiters,
           (unsigned long)numincrs,
           (total / i) - overhead,
           passed ? "PASSED" : "FAILED");
}

int main(int   argc,
         char *argv[])
{
    double overhead = 0;

    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    NUMARG(numincrs, "NUM_INCRS");
    NUMARG(numiters, "NUM_ITERS");
    iprintf("%i shepherds\n", qthread_num_shepherds());
    iprintf("%i threads\n", qthread_num_workers());

    sleeplen       = malloc(sizeof(size_t) * numincrs);
    random_numbers = malloc(sizeof(double) * numincrs);
    for (int i = 0; i < numincrs; ++i) {
        sleeplen[i]         = random() % 1000;
        random_numbers[i]   = ((double)random() / (double)RAND_MAX);
        known_correct.max   = (random_numbers[i] > known_correct.max) ? random_numbers[i] : known_correct.max;
        known_correct.min   = (random_numbers[i] > known_correct.min) ? known_correct.min : random_numbers[i];
        known_correct.sum  += random_numbers[i];
        known_correct.prod *= random_numbers[i];
        overhead           += sleeplen[i] / 1.0e6;
    }
    overhead         /= qthread_num_workers();
    known_correct.cnt = numincrs;
    known_correct.avg = known_correct.sum / known_correct.cnt;

    timer = qtimer_create();

    printf("%-21s %-9s %5s %8s %s\n", "version", "sync", "iters", "incrs", "time");
    run_iterations(qt_loopaccum_balance_sv, count, multi_op, overhead, "balanced", "syncvar");
    run_iterations(qt_loopaccum_balance_sinc, count, multi_op, overhead, "balanced", "sinc");
    run_iterations(qt_loopaccum_balance_dc, count, multi_op, overhead, "balanced", "donecount");

    return 0;
}

/* vim:set expandtab */
