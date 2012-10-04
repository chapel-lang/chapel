#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#ifdef HAVE_SYS_SYSCALL_H
# include <unistd.h>
# include <sys/syscall.h>        /* for SYS_accept and others */
#endif

/* Internal Headers */
#include "qt_io.h"
#include "qthread_asserts.h"
#include "qthread_innards.h" /* for qlib */
#include "qthread/qthread.h"
#include "qthread/qtimer.h"

unsigned int sleep(unsigned int seconds)
{
    if ((qlib != NULL) && (qthread_internal_self() != NULL)) {
        qtimer_t t = qtimer_create();
        qtimer_start(t);
        do {
            qthread_yield();
            qtimer_stop(t);
        } while (qtimer_secs(t) < seconds);
        return 0;
    } else {
#if HAVE_SYSCALL
# if HAVE_DECL_SYS_SLEEP
        return syscall(SYS_sleep, seconds);

# elif HAVE_DECL_SYS_USLEEP
        return syscall(SYS_usleep, seconds * 1e6);

# elif HAVE_DECL_SYS_NANOSLEEP
        return syscall(SYS_nanosleep, seconds * 1e9);

# else
        return 0;
# endif
#else   /* if HAVE_SYSCALL */
        return 0;
#endif  /* if HAVE_SYSCALL */
    }
}

/* vim:set expandtab: */
