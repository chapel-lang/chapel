#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#include <time.h>

#ifdef HAVE_SYS_SYSCALL_H
# include <unistd.h>
# include <sys/syscall.h>        /* for SYS_accept and others */
#endif

/* API Headers */
#include "qthread/qthread.h"
#include "qthread/qtimer.h"

/* Internal Headers */
#include "qt_io.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_qthread_mgmt.h"

int qt_nanosleep(const struct timespec *rqtp,
                 struct timespec       *rmtp)
{
    if (qt_blockable()) {
        qtimer_t t       = qtimer_create();
        double   seconds = rqtp->tv_sec + (rqtp->tv_nsec * 1e-9);

        qtimer_start(t);
        do {
            qthread_yield();
            qtimer_stop(t);
        } while (qtimer_secs(t) < seconds);
        if (rmtp) {
            time_t secs = (time_t)qtimer_secs(t);
            rmtp->tv_sec = rqtp->tv_sec - secs;
            rmtp->tv_nsec = (long)(qtimer_secs(t) - (double)secs) * 1e9;
        }
        qtimer_destroy(t);
        return 0;
    } else {
        if (rmtp) {
            *rmtp = *rqtp;
        }
        return -1;
    }
}

#if HAVE_SYSCALL && HAVE_DECL_SYS_NANOSLEEP
int nanosleep(const struct timespec *rqtp,
              struct timespec       *rmtp)
{
    if (qt_blockable()) {
      return qt_nanosleep(rqtp, rmtp);
    } else {
      return syscall(SYS_nanosleep, rqtp, rmtp);
    }
}

#endif /* if HAVE_SYSCALL && HAVE_DECL_SYS_NANOSLEEP */

/* vim:set expandtab: */
