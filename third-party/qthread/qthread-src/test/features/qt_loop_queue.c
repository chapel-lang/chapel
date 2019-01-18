#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>                    /* for INT_MIN & friends (according to C89) */
#include <float.h>                     /* for DBL_MIN & friends (according to C89) */
#include <sys/time.h>
#include <time.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static unsigned int BIGLEN = 1000000U;
static aligned_t *uia = NULL;
static unsigned int REALLY_VERBOSE = 0;

static void sum(const size_t startat,
                const size_t stopat,
                void *arg_)
{
    size_t i;
    aligned_t local_sum = 0, bigsum;

    if (REALLY_VERBOSE) {
        printf("S%i: summing %i numbers, from %i to %i\n",
               (int)qthread_shep(), (int)(stopat - startat), (int)startat,
               (int)stopat);
    }
    for (i = startat; i < stopat; ++i) {
        local_sum += uia[i];
    }
    bigsum = qthread_incr((aligned_t *)arg_, local_sum);
    if (REALLY_VERBOSE) {
        printf("S%i: localsum = %lu, bigsum = %lu\n", (int)qthread_shep(),
               (unsigned long)local_sum, (unsigned long)bigsum);
    }
}

static const char *units[] = {
    "bytes",
    "kB",
    "MB",
    "GB",
    "TB",
    "PB",
    "EB",
    NULL
};

static char *human_readable_bytes(size_t bytes)
{
    static char str[1024];

    int unit = 0;
    double dbytes = bytes;

    while (dbytes > 1024.0 && units[unit] != 0) {
        dbytes /= 1024.0;
        unit++;
    }
    snprintf(str, 1024, "%.2f %s", dbytes, units[unit]);
    return str;
}

int main(int argc,
         char *argv[])
{
    size_t i;

    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    NUMARG(BIGLEN, "BIGLEN");
    NUMARG(REALLY_VERBOSE, "REALLY_VERBOSE");
    // future_init(128);
    iprintf("%i threads\n", qthread_num_shepherds());

    {
        aligned_t uitmp = 0, uisum = 0;
        qqloop_handle_t *loophandle;
        qtimer_t t = qtimer_create();

        iprintf("allocating array of %u elements (%s)...\n", BIGLEN,
                human_readable_bytes(BIGLEN * sizeof(aligned_t)));
        uia = (aligned_t *)malloc(sizeof(aligned_t) * BIGLEN);
        assert(uia);
        iprintf("initializing array\n");
        for (i = 0; i < BIGLEN; i++) {
            uia[i] = random();
        }
        qtimer_start(t);
        for (i = 0; i < BIGLEN; i++) uisum += uia[i];
        qtimer_stop(t);
        iprintf("summing-serial   %u uints took %g seconds\n", BIGLEN,
                qtimer_secs(t));
        iprintf("\tsum was %lu\n", (unsigned long)uisum);

        loophandle = qt_loop_queue_create(CHUNK, 0, BIGLEN, 1, sum, &uitmp);
        qtimer_start(t);
        qt_loop_queue_run(loophandle);
        qtimer_stop(t);
        iprintf("summing-parallel CHUNK %u uints took %g seconds\n", BIGLEN,
                qtimer_secs(t));
        iprintf("\tsum was %lu\n", (unsigned long)uitmp);
        assert(uitmp == uisum);

        uitmp = 0;
        loophandle = qt_loop_queue_create(GUIDED, 0, BIGLEN, 1, sum, &uitmp);
        qtimer_start(t);
        qt_loop_queue_run(loophandle);
        qtimer_stop(t);
        iprintf("summing-parallel GUIDED %u uints took %g seconds\n", BIGLEN,
                qtimer_secs(t));
        iprintf("\tsum was %lu\n", (unsigned long)uitmp);
        assert(uitmp == uisum);

        uitmp = 0;
        loophandle = qt_loop_queue_create(FACTORED, 0, BIGLEN, 1, sum, &uitmp);
        qtimer_start(t);
        qt_loop_queue_run(loophandle);
        qtimer_stop(t);
        iprintf("summing-parallel FACTORED %u uints took %g seconds\n", BIGLEN,
                qtimer_secs(t));
        iprintf("\tsum was %lu\n", (unsigned long)uitmp);
        assert(uitmp == uisum);

        uitmp = 0;
        loophandle = qt_loop_queue_create(TIMED, 0, BIGLEN, 1, sum, &uitmp);
        qtimer_start(t);
        qt_loop_queue_run(loophandle);
        qtimer_stop(t);
        iprintf("summing-parallel TIMED %u uints took %g seconds\n", BIGLEN,
                qtimer_secs(t));
        iprintf("\tsum was %lu\n", (unsigned long)uitmp);
        assert(uitmp == uisum);

        free(uia);
        qtimer_destroy(t);
    }

    return 0;
}

/* vim:set expandtab */
