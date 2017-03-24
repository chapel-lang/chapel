#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>

#include <qthread/qtimer.h>

#include "qt_debug.h" /* for malloc debug wrappers */

struct qtimer_s {
    hrtime_t start;
    hrtime_t stop;
};

void qtimer_start(qtimer_t q)
{
    q->start = gethrtime();
}

unsigned long qtimer_fastrand(void)
{
    return (unsigned long)(gethrtime());
}

void qtimer_stop(qtimer_t q)
{
    q->stop = gethrtime();
}

double qtimer_wtime(void)
{
    return ((double)gethrtime()) * 1e-9;
}

double qtimer_res(void)
{
#if defined(HAVE_SYSCONF) && defined(HAVE_SC_CLK_TCK)
    return 1.0 / sysconf(_SC_CLK_TCK);;
#else
    return 1e-9;
#endif
}

double qtimer_secs(qtimer_t q)
{
    return ((double)(q->stop - q->start)) * 1e-9;
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
