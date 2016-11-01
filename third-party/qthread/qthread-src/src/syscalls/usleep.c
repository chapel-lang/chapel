#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#ifdef HAVE_SYS_SYSCALL_H
# include <unistd.h>
# include <sys/syscall.h>        /* for SYS_accept and others */
#endif

/* Public Headers */
#include "qthread/qthread.h"
#include "qthread/qtimer.h"

/* Internal Headers */
#include "qt_io.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_qthread_mgmt.h"

int usleep(useconds_t useconds)
{
    if ((qlib != NULL) && (qthread_internal_self() != NULL)) {
        qtimer_t t       = qtimer_create();
        double   seconds = useconds * 1e-6;
        qtimer_start(t);
        do {
            qthread_yield();
            qtimer_stop(t);
        } while (qtimer_secs(t) < seconds);
        qtimer_destroy(t);
        return 0;
    } else {
#if HAVE_SYSCALL
# if HAVE_DECL_SYS_USLEEP
        return syscall(SYS_usleep, useconds);

# elif HAVE_DECL_SYS_NANOSLEEP
        return syscall(SYS_nanosleep, useconds * 1e3);

# else
        return 0;
# endif
#else
        return 0;
#endif  /* if HAVE_SYSCALL */
    }
}

/* vim:set expandtab: */
