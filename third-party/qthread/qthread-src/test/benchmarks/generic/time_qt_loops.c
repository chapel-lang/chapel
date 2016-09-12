#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static aligned_t threads  = 0;
static aligned_t numincrs = 1024;
static size_t   *randlen;
static qtimer_t  timer;
static size_t    numiters = 10;

static int num_sheps = 0;
static int num_workers = 0;

static void sumrand(const size_t startat,
                    const size_t stopat,
                    void        *arg_)
{
    size_t tmp, tmp2;
    qthread_incr(&threads, stopat - startat);
    for (size_t i = startat; i < stopat; ++i) {
        tmp = randlen[i];
        tmp2 = tmp;
        while (tmp2 > 0) {
            tmp += qtimer_fastrand();
            tmp2--;
        }
    }
}

static void sum(const size_t startat,
                const size_t stopat,
                void        *arg_)
{
    qthread_incr(&threads, stopat - startat);
}

typedef void (*loop_f)(const size_t a,
                       const size_t    b,
                       const qt_loop_f f,
                       void           *c);

typedef struct run_args_s {
    loop_f      loop;
    qt_loop_f   func;
    const char *type;
    const char *name;
} run_args_t;

static aligned_t run_iterations(void *arg_)
{
    run_args_t *arg = (run_args_t *)arg_;
    loop_f      loop = arg->loop;
    qt_loop_f   func = arg->func;
    const char *type = arg->type;
    const char *name = arg->name;

    double total = 0;

    for (int i = 0; i < numiters; ++i) {
        threads = 0;
        qtimer_start(timer);
        loop(0, numincrs, func, NULL);
        assert(threads == numincrs);
        qtimer_stop(timer);
        total += qtimer_secs(timer);
    }
    printf("%5i %7i %-23s %-9s %8lu %f\n", num_sheps, num_workers, type, name, (unsigned long)numincrs,
           (total / numiters));

    return 0;
}

int main(int   argc,
         char *argv[])
{
    int    print_headers = 1;

    assert(qthread_initialize() == QTHREAD_SUCCESS);
    num_sheps   = qthread_num_shepherds();
    num_workers = qthread_num_workers();

    CHECK_VERBOSE();
    NUMARG(numincrs, "NUM_INCRS");
    NUMARG(numiters, "NUM_ITERS");
    NUMARG(print_headers, "PRINT_HEADERS");
    if (print_headers) {
        printf("%i shepherds\n", num_sheps);
        printf("%i threads\n", num_workers);
    }
    timer = qtimer_create();

    aligned_t ret;

    randlen = malloc(sizeof(size_t) * numincrs);
    for (int i = 0; i < numincrs; ++i) {
        randlen[i] = qtimer_fastrand() % 10;
    }

    if (print_headers) {
        printf("Pure Increment\n");
        printf("%-4s %-4s %-23s %-9s %8s time\n", "sheps", "workers", "grouping work looptype", "sync", "iters");
    }

    qt_loop(0, numincrs, sum, NULL);

    run_args_t pure_args[8] = {
        {qt_loop_dc,              sum, "solo pure TPI",      "donecount"},
        {qt_loop_aligned,         sum, "solo pure TPI",      "aligned"},
        {qt_loop_sv,              sum, "solo pure TPI",      "syncvar"},
        {qt_loop_sinc,            sum, "solo pure TPI",      "sinc"},
        {qt_loop_balance_dc,      sum, "solo pure balanced", "donecount"},
        {qt_loop_balance_aligned, sum, "solo pure balanced", "aligned"},
        {qt_loop_balance_sv,      sum, "solo pure balanced", "syncvar"},
        {qt_loop_balance_sinc,    sum, "solo pure balanced", "sinc"},
    };

    for (int i = 0; i < 8; i++) {
        qthread_fork(run_iterations, &pure_args[i], &ret);
        qthread_readFE(NULL, &ret);
    }

    if (print_headers) {
        printf("Teamed Pure Increment\n");
        printf("%-4s %-4s %-23s %-9s %8s time\n", "sheps", "workers", "grouping work looptype", "sync", "iters");
    }

    qt_loop(0, numincrs, sum, NULL);

    run_args_t team_pure_args[8] = {
        {qt_loop_dc,              sum, "team pure TPI",      "donecount"},
        {qt_loop_aligned,         sum, "team pure TPI",      "aligned"},
        {qt_loop_sv,              sum, "team pure TPI",      "syncvar"},
        {qt_loop_sinc,            sum, "team pure TPI",      "sinc"},
        {qt_loop_balance_dc,      sum, "team pure balanced", "donecount"},
        {qt_loop_balance_aligned, sum, "team pure balanced", "aligned"},
        {qt_loop_balance_sv,      sum, "team pure balanced", "syncvar"},
        {qt_loop_balance_sinc,    sum, "team pure balanced", "sinc"},
    };

    for (int i = 0; i < 8; i++) {
        qthread_fork_new_team(run_iterations, &team_pure_args[i], &ret);
        qthread_readFE(NULL, &ret);
    }

    if (print_headers) {
        printf("\n");
        printf("Increment with Rand\n");
        printf("%-4s %-4s %-23s %-9s %8s time\n", "sheps", "workers", "grouping work looptype", "sync", "iters");
    }

    run_args_t rand_args[8] = {
        {qt_loop_dc,              sumrand, "solo rand TPI",      "donecount"},
        {qt_loop_aligned,         sumrand, "solo rand TPI",      "aligned"},
        {qt_loop_sv,              sumrand, "solo rand TPI",      "syncvar"},
        {qt_loop_sinc,            sumrand, "solo rand TPI",      "sinc"},
        {qt_loop_balance_dc,      sumrand, "solo rand balanced", "donecount"},
        {qt_loop_balance_aligned, sumrand, "solo rand balanced", "aligned"},
        {qt_loop_balance_sv,      sumrand, "solo rand balanced", "syncvar"},
        {qt_loop_balance_sinc,    sumrand, "solo rand balanced", "sinc"},
    };
    
    for (int i = 0; i < 8; i++) {
        qthread_fork(run_iterations, &rand_args[i], &ret);
        qthread_readFE(NULL, &ret);
    }

    if (print_headers) {
        printf("\n");
        printf("Team Increment with Rand\n");
        printf("%-4s %-4s %-23s %-9s %8s time\n", "sheps", "workers", "grouping work looptype", "sync", "iters");
    }

    run_args_t team_rand_args[8] = {
        {qt_loop_dc,              sumrand, "team rand TPI",      "donecount"},
        {qt_loop_aligned,         sumrand, "team rand TPI",      "aligned"},
        {qt_loop_sv,              sumrand, "team rand TPI",      "syncvar"},
        {qt_loop_sinc,            sumrand, "team rand TPI",      "sinc"},
        {qt_loop_balance_dc,      sumrand, "team rand balanced", "donecount"},
        {qt_loop_balance_aligned, sumrand, "team rand balanced", "aligned"},
        {qt_loop_balance_sv,      sumrand, "team rand balanced", "syncvar"},
        {qt_loop_balance_sinc,    sumrand, "team rand balanced", "sinc"},
    };
    
    for (int i = 0; i < 8; i++) {
        qthread_fork_new_team(run_iterations, &team_rand_args[i], &ret);
        qthread_readFE(NULL, &ret);
    }

    qtimer_destroy(timer);
    return 0;
}

/* vim:set expandtab */
