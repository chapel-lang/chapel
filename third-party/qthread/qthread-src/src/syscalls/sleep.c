#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#ifdef HAVE_SYS_SYSCALL_H
# include <unistd.h>
# include <sys/syscall.h>        /* for SYS_accept and others */
#endif

/* API Headers */
#include "qthread/qthread.h"
#include "qthread/qtimer.h"

/* Internal Headers */
#include "qt_io.h"
#include "qt_asserts.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_qthread_mgmt.h"

unsigned int qt_sleep(unsigned int seconds)
{
    if (qt_blockable()) {
        qtimer_t t = qtimer_create();
        qtimer_start(t);
        do {
            qthread_yield();
            qtimer_stop(t);
        } while (qtimer_secs(t) < seconds);
        qtimer_destroy(t);
        return 0;
    } else {
        return seconds;
    }
}

#if HAVE_SYSCALL && HAVE_DECL_SYS_SLEEP
unsigned int sleep(unsigned int seconds)
{
  if (qt_blockable()) {
    return qt_sleep(seconds);
  } else {
    return syscall(SYS_sleep, seconds);
  }
}

#endif /* if HAVE_SYSCALL && HAVE_DECL_SYS_SLEEP */

/* vim:set expandtab: */
