#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <qthread/qtimer.h>
#include "qthread_asserts.h"

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

struct qtimer_s {
    struct timespec start, stop;
};

void qtimer_start(qtimer_t q)
{
    assert(q);
    qassert(clock_gettime(CLOCK_MONOTONIC, &(q->start)), 0);
}

long qtimer_fastrand(void)
{
    struct timespec s;

    qassert(clock_gettime(CLOCK_MONOTONIC, &(s)), 0);
    return (long)(s.tv_nsec >> 5); // last five bits seem to always be the same
}

void qtimer_stop(qtimer_t q)
{
    assert(q);
    qassert(clock_gettime(CLOCK_MONOTONIC, &(q->stop)), 0);
}

double qtimer_secs(qtimer_t q)
{
    assert(q);
    return (q->stop.tv_sec + q->stop.tv_nsec * 1e-9) - (q->start.tv_sec + q->start.tv_nsec * 1e-9);
}

qtimer_t qtimer_create()
{
    qtimer_t ret = calloc(1, sizeof(struct qtimer_s));

    assert(ret);
    return ret;
}

void qtimer_destroy(qtimer_t q)
{
    assert(q);
    free(q);
}

/* vim:set expandtab: */
