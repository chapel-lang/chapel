#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <qthread/qtimer.h>

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

#include "qt_debug.h" /* for malloc debug wrappers */

struct qtimer_s {
    struct timeval start, stop;
};

void qtimer_start(qtimer_t q)
{
    gettimeofday(&(q->start), NULL);
}

unsigned long qtimer_fastrand(void)
{
    struct timeval s;

    gettimeofday(&(s), NULL);
    return (unsigned long)(s.tv_usec);
}

double qtimer_wtime(void)
{
    struct timeval s;

    gettimeofday(&(s), NULL);
    return s.tv_sec + (s.tv_usec * 1e-6);
}

double qtimer_res(void)
{
#if defined(HAVE_SYSCONF) && defined(HAVE_SC_CLK_TCK)
    return 1.0 / sysconf(_SC_CLK_TCK);;
#else
    return 1e-9;
#endif
}

void qtimer_stop(qtimer_t q)
{
    gettimeofday(&(q->stop), NULL);
}

double qtimer_secs(qtimer_t q)
{
    return (q->stop.tv_sec + q->stop.tv_usec * 1e-6) - (q->start.tv_sec + q->start.tv_usec * 1e-6);
}

qtimer_t qtimer_create()
{
    return qt_calloc(1, sizeof(struct qtimer_s));
}

void qtimer_destroy(qtimer_t q)
{
    FREE(q, sizeof(struct qtimer_s));
}

/* vim:set expandtab: */
