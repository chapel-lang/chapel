#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <qthread/qthread-int.h> /* for uint64_t */

#ifdef HAVE_SYS_SYSCALL_H
# include <unistd.h>
# include <sys/syscall.h>        /* for SYS_accept and others */
#endif

#if HAVE_SYSCALL && HAVE_DECL_SYS_WAIT4
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>
#endif

/* Public Headers */
#include "qthread/qt_syscalls.h"

/* Internal Headers */
#include "qt_io.h"
#include "qt_asserts.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_qthread_mgmt.h"

pid_t qt_wait4(pid_t          pid,
               int           *stat_loc,
               int            options,
               struct rusage *rusage)
{
    qthread_t                *me  = qthread_internal_self();
    qt_blocking_queue_node_t *job = ALLOC_SYSCALLJOB();
    pid_t                     ret;

    assert(job);
    job->next   = NULL;
    job->thread = me;
    job->op     = WAIT4;
    memcpy(&job->args[0], &pid, sizeof(pid_t));
    job->args[1] = (uintptr_t)stat_loc;
    memcpy(&job->args[2], &options, sizeof(int));
    job->args[3] = (uintptr_t)rusage;

    assert(me->rdata);

    me->rdata->blockedon.io = job;
    me->thread_state        = QTHREAD_STATE_SYSCALL;
    qthread_back_to_master(me);
    ret = job->ret;
    FREE_SYSCALLJOB(job);
    return ret;
}

#if HAVE_SYSCALL && HAVE_DECL_SYS_WAIT4
pid_t wait4(pid_t          pid,
            int           *stat_loc,
            int            options,
            struct rusage *rusage)
{
    if ((qlib != NULL) && (qthread_internal_self() != NULL)) {
        return qt_wait4(pid, stat_loc, options, rusage);
    } else {
        return syscall(SYS_wait4, pid, stat_loc, options, rusage);
    }
}

#endif /* if HAVE_SYSCALL && HAVE_DECL_SYS_WAIT4 */

/* vim:set expandtab: */
