#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <qthread/qtimer.h>

#include <stdlib.h> /* calloc() & free()*/

#include "qthread-int.h"
#include <mach/mach_time.h>

struct qtimer_s {
    uint64_t start, stop;
};

void qtimer_start(qtimer_t q)
{
    q->start = mach_absolute_time();
}

unsigned long qtimer_fastrand(void)
{
    return (unsigned long)(mach_absolute_time());
}

void qtimer_stop(qtimer_t q)
{
    q->stop = mach_absolute_time();
}

double qtimer_secs(qtimer_t q)
{
    uint64_t      difference = q->stop - q->start;
    static double conversion = 0.0;

    if (conversion == 0.0) {
        mach_timebase_info_data_t info;
        kern_return_t             err = mach_timebase_info(&info);

        // Convert the timebase into seconds
        if (err == 0) {
            conversion = 1e-9 * (double)info.numer / (double)info.denom;
        }
    }
    return conversion * (double)difference;
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
