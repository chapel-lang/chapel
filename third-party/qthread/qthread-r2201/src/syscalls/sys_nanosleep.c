#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <time.h>

#include <unistd.h>
#include <sys/syscall.h>         /* for SYS_accept and others */

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h" /* for qlib */
#include "qthread/qthread.h"
#include "qthread/qtimer.h"

int nanosleep(const struct timespec *rqtp,
              struct timespec       *rmtp)
{
    if ((qlib != NULL) && (qthread_internal_self() != NULL)) {
        qtimer_t t       = qtimer_create();
        double   seconds = rqtp->tv_sec + (rqtp->tv_nsec * 1e-9);

        qtimer_start(t);
        do {
            qthread_yield();
            qtimer_stop(t);
        } while (qtimer_secs(t) < seconds);
        return 0;
    } else {
#if HAVE_DECL_SYS_NANOSLEEP
        return syscall(SYS_nanosleep, rqtp, rmtp);

#else
        return 0;
#endif
    }
}

/* vim:set expandtab: */
