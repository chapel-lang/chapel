#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>

#include <qthread/qtimer.h>

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

double qtimer_secs(qtimer_t q)
{
    return ((double)(q->stop - q->start)) * 1e-9;
}

qtimer_t qtimer_create()
{
    return calloc(1, sizeof(struct qtimer_s));
}

void qtimer_destroy(qtimer_t q)
{
    free(q);
}

/* vim:set expandtab: */
